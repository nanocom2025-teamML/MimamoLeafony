from linebot import LineBotApi
from linebot.models import TextSendMessage, TemplateSendMessage, ConfirmTemplate, PostbackAction
from app.config import CHANNEL_ACCESS_TOKEN

line_bot_api = LineBotApi(CHANNEL_ACCESS_TOKEN)

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
        text="門限を変更しますか？",
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

def handle_message(reply_token: str, text: str):
    """
    ユーザーからのテキストメッセージ処理
    """
    reply_message(reply_token, f"受信しました: {text}")

def handle_postback(data: str, params: dict, reply_token: str):
    """
    ポストバック処理
    """
    # 見守り者のメッセージ送信
    if data == "action=send_message":
        reply_message(reply_token, "送りたいメッセージを入力してください")
        # TODO: DBに「入力待ち状態」を保存
        # TODO: targetsテーブルに入力待ち状態（state）を追加？

    # 門限時刻の登録
    elif data == "action=set_curfew":
        time = params.get("time") if params else None
        if time:
            reply_message(reply_token, f"門限を {time} に登録しました！")
            # TODO: DBに保存処理
        else:
            reply_message(reply_token, "門限の時間が指定されませんでした。")

    elif data == "confirm=yes":
        reply_message(reply_token, "門限超過を取り消しました。")
        # TODO: DBに保存処理
    
    elif data == "confirm=no":
        reply_message(reply_token, "門限超過の取り消しをキャンセルしました。")