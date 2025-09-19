from linebot import LineBotApi
from linebot.models import TextSendMessage, TemplateSendMessage, ConfirmTemplate, PostbackAction
from app.config import CHANNEL_ACCESS_TOKEN

line_bot_api = LineBotApi(CHANNEL_ACCESS_TOKEN)


def reply_message(reply_token: str, text: str):
    """
    メッセージを返信する
    """
    line_bot_api.reply_message(
        reply_token,
        TextSendMessage(text=text)
    )


def push_message(user_id: str, text: str):
    """
    指定ユーザーにメッセージを送信する
    """
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
    指定ユーザーに門限用確認テンプレートを送信する
    """
    template = ConfirmTemplate(
        text=text,
        actions=[
            PostbackAction(label="はい", data="confirm=yes"),
            PostbackAction(label="いいえ", data="confirm=no")
        ]
    )
    message = TemplateSendMessage(alt_text=text, template=template)
    
    try:
        line_bot_api.push_message(user_id, message)
        print(f"ConfirmTemplate を {user_id} に送信")
    except Exception as e:
        print(f"送信エラー: {e}")


