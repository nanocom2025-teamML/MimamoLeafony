from linebot import AsyncLineBotApi, WebhookParser
from linebot.models import MessageEvent, TextMessage, TextSendMessage
from app.config import settings
from app.services import db_service

line_bot_api = AsyncLineBotApi(settings.CHANNEL_ACCESS_TOKEN)
parser = WebhookParser(settings.CHANNEL_SECRET)

async def handle_event(body: str, signature: str):
    events = parser.parse(body, signature)

    for event in events:
        if isinstance(event, MessageEvent) and isinstance(event.message, TextMessage):
            text = event.message.text.strip()

            if text.startswith("門限を設定"):
                # 例: 「門限を設定 21:00」
                parts = text.split()
                if len(parts) == 2:
                    db_service.set_curfew(event.source.user_id, parts[1])
                    reply_text = f"門限を {parts[1]} に設定しました。"
                else:
                    reply_text = "「門限を設定 21:00」のように入力してください。"
            elif text == "ヘルプ":
                reply_text = "門限を設定するには「門限を設定 21:00」と入力してください。"
            else:
                # エコー
                reply_text = f"受け取りました: {text}"

            await line_bot_api.reply_message(
                event.reply_token,
                TextSendMessage(text=reply_text)
            )
