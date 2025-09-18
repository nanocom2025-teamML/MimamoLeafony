from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from datetime import datetime

from app.database import get_db
from app.models import Access, Message
from app.services.recognize_audio_service import convert_to_wav, recognize_voice, append_csv, delete_csv
from app.services.line_message_service import push_message

from app.config import LINE_USER_ID, TARGET_ID


router = APIRouter(prefix="/api/device", tags=["device"])


@router.post("/messages")
def post_device_messages(db: Session = Depends(get_db)):
    text: str = ""

    try:
        text: str = recognize_voice(convert_to_wav())
        
        print(text)

        # text = "いってきます"

        now = datetime.utcnow()

        if any(word in text for word in ["いってきます", "行って来ます", "行ってきます", "いって来ます", "いってきま", "行ってきま"]):
            # 新規Accessの作成
            new_access = Access(
                target_id=TARGET_ID,
                gone_at=now,
                come_at=None,
            )
            db.add(new_access)
            db.commit()
            db.refresh(new_access)

            push_message(LINE_USER_ID, f"{text}（外出を検知しました）")

        elif any(word in text for word in ["ただいま", "只今", "ただ今", "只いま"]):
            # 直近のAccessの参照
            last_access = (
                db.query(Access)
                .filter(Access.come_at.is_(None))
                .order_by(Access.gone_at.desc())
                .first()
            )

            if last_access:
                last_access.come_at = now
                db.commit()
                db.refresh(last_access)
            else:
                pass

            push_message(LINE_USER_ID, f"{text}（帰宅を検知しました）")

        elif text is None or text == "":
            push_message(LINE_USER_ID, f"音声を認識できませんでした")

        else:
            push_message(LINE_USER_ID, text)

        return {"text": text}

    except Exception as e:
        return {"error": f"音声認識に失敗しました: {e}"}
    finally:
        delete_csv()


# 非推奨（MQTTを使いましょう）
@router.post("/audio-data")
def post_device_audio_data(data: dict, db: Session = Depends(get_db)):
    """
    音声データをCSVファイルに逐次追記するAPI
    """
    audio_data = data["audio_data"]

    append_csv(audio_data)

    return {"message": "Audio data appended", "samples_received": len(audio_data)}


@router.post("/touch")
def post_device_touch(db: Session = Depends(get_db)):
    # 未読メッセージの取得
    unread_messages = db.query(Message).filter(Message.read_at.is_(None)).all()

    if not unread_messages:
        return {"messages": []}

    # レスポンス用のデータ
    response_data = [
        {
            "id": msg.id,
            "target_id": msg.target_id,
            "content": msg.content,
            "read_at": msg.read_at,
            "created_at": msg.created_at,
            "updated_at": msg.updated_at,
        }
        for msg in unread_messages
    ]

    # 既読に変更する
    now = datetime.utcnow()
    for msg in unread_messages:
        msg.read_at = now
    db.commit()

    return {"messages": response_data}
