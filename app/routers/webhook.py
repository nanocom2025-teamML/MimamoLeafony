from fastapi import APIRouter, Request, HTTPException, Depends
from sqlalchemy.orm import Session
from linebot import LineBotApi, WebhookParser
from linebot.exceptions import InvalidSignatureError
from linebot.models import MessageEvent, TextMessage, PostbackEvent

from app.config import CHANNEL_ACCESS_TOKEN, CHANNEL_SECRET
from app.services import line_service
from app.database import get_db

router = APIRouter()

line_bot_api = LineBotApi(CHANNEL_ACCESS_TOKEN)
parser = WebhookParser(CHANNEL_SECRET)

# POST /webhook
@router.post("/webhook")
async def line_webhook(request: Request , db: Session = Depends(get_db)):
    signature = request.headers["X-Line-Signature"]
    if signature is None:
        raise HTTPException(status_code=400, detail="Missing X-Line-Signature header")

    body = await request.body()

    try:
        events = parser.parse(body.decode("utf-8"), signature)
    except InvalidSignatureError:
        raise HTTPException(status_code=400, detail="Invalid signature")

    for event in events:
        # メッセージイベント,テキストイベントの処理
        if isinstance(event, MessageEvent) and isinstance(event.message, TextMessage):
            line_service.handle_message(event.reply_token, event.message.text, db)

        # ポストバックイベントの処理
        if isinstance(event, PostbackEvent):
            line_service.handle_postback(
                data=event.postback.data,
                params=event.postback.params,
                reply_token=event.reply_token,
                db=db
            )

    return "OK"