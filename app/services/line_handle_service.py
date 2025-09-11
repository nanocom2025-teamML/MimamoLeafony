from sqlalchemy.orm import Session
from app.services import db_service, line_message_service
from datetime import datetime

from app.config import TARGET_ID


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

            line_message_service.reply_message(reply_token, f"以下のメッセージを送りました\n\n{text}")
        # 門限時刻待ち状態の場合
        elif awaiting_state == "waiting_for_curfew":
            # TODO: 動作確認後コメントアウト
            line_message_service.reply_message(reply_token, f"門限登録をキャンセルしました")
        return

    # 状態なしは通常のオウム返し
    # TODO: 動作確認後コメントアウト
    line_message_service.reply_message(reply_token, f"受信しました: {text}")


def handle_postback(data: str, params: dict, reply_token: str, db: Session):
    """
    ポストバック処理
    """
    # 見守り者のメッセージ送信
    if data == "action=send_message":
        # Targetテーブルを「入力待ち状態」に変更
        db_service.update_awaiting_state(db, TARGET_ID, "waiting_for_message")
        line_message_service.reply_message(reply_token, "送りたいメッセージを入力してください")
        return
        
    # 門限時刻の登録
    if data == "action=set_curfew":
        time_str = params.get("time") if params else None
        if time_str:
            # Targetテーブルに門限時刻を保存
            curfew_time = datetime.strptime(time_str, "%H:%M").time()
            db_service.update_curfew(db, TARGET_ID, curfew_time)
            line_message_service.reply_message(reply_token, f"門限を {curfew_time} に登録しました！")

        else:
            line_message_service.reply_message(reply_token, "門限の時間が指定されませんでした。")
        return

    awaiting_state = db_service.get_awaiting_state(db, TARGET_ID)

    # 門限確認（Yes/No）
    if awaiting_state == "waiting_for_curfew":
        if data == "confirm=yes":
            line_message_service.reply_message(reply_token, "門限超過を取り消しました。")
            # TODO: DBに帰宅時間を保存？

            db_service.update_awaiting_state(db, TARGET_ID, None)
            return
        elif data == "confirm=no":
            line_message_service.reply_message(reply_token, "速やかに連絡を取り、帰宅を促しましょう")
            db_service.update_awaiting_state(db, TARGET_ID, None)
            return
