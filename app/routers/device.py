from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from datetime import datetime

from app.database import get_db
from app.models import Access, Message
from app.services.recognize_audio_service import convert_to_wav, recognize_voice


router = APIRouter(prefix="/api/device", tags=["device"])


@router.post("/messages")
def post_device_message(data: dict, db: Session = Depends(get_db)):
    audio_data = data["audio_data"]
    text: str = recognize_voice(convert_to_wav(audio_data))
    print(text)

    # text = "いってきます"

    now = datetime.utcnow()

    if any(word in text for word in ["いって", "行って", "きます", "来ます"]):
        # 新規Accessの作成
        new_access = Access(
            target_id=1,
            gone_at=now,
            come_at=None,
        )
        db.add(new_access)
        db.commit()
        db.refresh(new_access)

    elif any(word in text for word in ["ただ", "いま"]):
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

    return {"text": text}


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
