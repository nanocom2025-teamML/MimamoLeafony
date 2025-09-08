from sqlalchemy.orm import Session
from linebot import LineBotApi
from linebot.models import TextSendMessage, TemplateSendMessage, ConfirmTemplate, PostbackAction
from app.config import CHANNEL_ACCESS_TOKEN
from app.services import db_service

line_bot_api = LineBotApi(CHANNEL_ACCESS_TOKEN)

TARGET_ID = 1

def reply_message(reply_token: str, text: str):
    line_bot_api.reply_message(
        reply_token,
        TextSendMessage(text=text)
    )

def push_message(user_id: str, text: str):
    line_bot_api.push_message(
        user_id,
        TextSendMessage(text=text)
    )

def broadcast_message(text: str):
    """
    全ユーザーにメッセージを一斉配信する
    """
    try:
        line_bot_api.broadcast(
            messages=[TextSendMessage(text=text)]
        )
        print("Broadcast 成功")
    except Exception as e:
        print(f"Broadcast エラー: {e}")

def push_confirm_template(user_id: str, text: str):
    """
    指定ユーザーに確認テンプレートを送る
    """
    template = ConfirmTemplate(
        text="門限を過ぎました。\nすでに帰宅していますか？",
        actions=[
            PostbackAction(label="はい", data="confirm=yes"),
            PostbackAction(label="いいえ", data="confirm=no")
        ]
    )
    message = TemplateSendMessage(alt_text=text, template=template)
    
    try:
        push_message(user_id, message)
        print(f"ConfirmTemplate を {user_id} に送信")
    except Exception as e:
        print(f"送信エラー: {e}")


def handle_message(reply_token: str, text: str, db: Session):
    """
    見守り者からのテキストメッセージ処理
    """
    awaiting_state = db_service.get_awaiting_state(db, TARGET_ID)
     # 状態がある場合はリセット
    if awaiting_state:
        # TODO Targetテーブルのデータを2度取得しているので修正
        db_service.update_awaiting_state(db, TARGET_ID, None)
        # メッセージ入力待ち状態の場合
        if awaiting_state == "waiting_for_message":
            # DBに新規メッセージを作成
            db_service.create_message(db, TARGET_ID, text)
            # TODO: デバイスに新規メッセージの通知

            reply_message(reply_token, f"以下のメッセージを送りました\n\n{text}")
        # 門限時刻待ち状態の場合
        elif awaiting_state == "waiting_for_curfew":
            # TODO: 動作確認後コメントアウト
            reply_message(reply_token, f"門限登録をキャンセルしました")
        return

    # 状態なしは通常のオウム返し
    # TODO: 動作確認後コメントアウト
    reply_message(reply_token, f"受信しました: {text}")


def handle_postback(data: str, params: dict, reply_token: str, db: Session):
    """
    ポストバック処理
    """
    # 見守り者のメッセージ送信
    if data == "action=send_message":
        # Targetテーブルを「入力待ち状態」に変更
        db_service.update_awaiting_state(db, TARGET_ID, "waiting_for_message")
        reply_message(reply_token, "送りたいメッセージを入力してください")
        return
        
    # 門限時刻の登録
    if data == "action=set_curfew":
        time = params.get("time") if params else None
        if time:
            # Targetテーブルに門限時刻を保存
            db_service.update_curfew(db, TARGET_ID, time)
            reply_message(reply_token, f"門限を {time} に登録しました！")
            
        else:
            reply_message(reply_token, "門限の時間が指定されませんでした。")
        return

    awaiting_state = db_service.get_awaiting_state(db, TARGET_ID)

    # 門限確認（Yes/No）
    if awaiting_state == "waiting_for_curfew":
        if data == "confirm=yes":
            reply_message(reply_token, "門限超過を取り消しました。")
            # TODO: DBに帰宅時間を保存？

            db_service.update_awaiting_state(db, TARGET_ID, None)
            return
        elif data == "confirm=no":
            reply_message(reply_token, "速やかに連絡を取り、帰宅を促しましょう")
            db_service.update_awaiting_state(db, TARGET_ID, None)
            return
