from fastapi import APIRouter, Request, HTTPException
from linebot import LineBotApi, WebhookParser
from linebot.exceptions import InvalidSignatureError
from linebot.models import MessageEvent, TextMessage, TextSendMessage, PostbackEvent

from app.config import CHANNEL_ACCESS_TOKEN, CHANNEL_SECRET

router = APIRouter()

line_bot_api = LineBotApi(CHANNEL_ACCESS_TOKEN)
parser = WebhookParser(CHANNEL_SECRET)

@router.post("/webhook")
async def callback(request: Request):
    signature = request.headers["X-Line-Signature"]
    body = await request.body()

    try:
        events = parser.parse(body.decode("utf-8"), signature)
    except InvalidSignatureError:
        raise HTTPException(status_code=400, detail="Invalid signature")

    for event in events:
        if isinstance(event, MessageEvent) and isinstance(event.message, TextMessage):
            line_bot_api.reply_message(
                event.reply_token,
                TextSendMessage(text=f"受信しました: {event.message.text}")
            )

        if isinstance(event, PostbackEvent):
            data = event.postback.data
            params = event.postback.params
            # 例: 門限時刻設定
            if data == "action=setCurfew":
                time = params.get("time")
                line_bot_api.reply_message(
                    event.reply_token,
                    TextSendMessage(text=f"門限を {time} に設定しました！")
                )

    return "OK"
