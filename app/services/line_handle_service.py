from sqlalchemy.orm import Session
from app.services import db_service, line_message_service
from datetime import datetime
from app.jobs.curfew_job import scheduler, check_curfew_job

from app.config import TARGET_ID


def handle_message(reply_token: str, text: str, db: Session):
    """
    見守り者からのテキストメッセージ処理
    """
    awaiting_state = db_service.get_awaiting_state(db, TARGET_ID)
     # 状態がある場合はリセット
    if awaiting_state:
        db_service.update_awaiting_state(db, TARGET_ID, None)
        # メッセージ入力待ち状態の場合
        if awaiting_state == "awaiting_message":
            # DBに新規メッセージを作成
            db_service.create_message(db, TARGET_ID, text)

            line_message_service.reply_message(reply_token, f"以下のメッセージを送りました\n\n{text}")
        # 門限時刻待ち状態の場合
        elif awaiting_state == "awaiting_curfew_time":
            # line_message_service.reply_message(reply_token, f"門限超過をキャンセルしました")
            pass
        return

    # 状態なしは通常のオウム返し
    # line_message_service.reply_message(reply_token, f"受信しました: {text}")


def handle_postback(data: str, params: dict, reply_token: str, db: Session):
    """
    ポストバック処理
    """
    awaiting_state = db_service.get_awaiting_state(db, TARGET_ID)
    if awaiting_state:
        db_service.update_awaiting_state(db, TARGET_ID, None)
        # 門限確認（Yes/No）
        if awaiting_state == "awaiting_curfew_time":
            if data == "confirm=yes":
                line_message_service.reply_message(reply_token, "門限超過を取り消しました。")
                # DBに帰宅時間を保存
                db_service.update_curfew_return(db, TARGET_ID)
                return
            elif data == "confirm=no":
                line_message_service.reply_message(reply_token, "速やかに連絡を取り、帰宅を促しましょう")
                return

    # 見守り者のメッセージ送信
    if data == "action=send_message":
        # Targetテーブルを「入力待ち状態」に変更
        db_service.update_awaiting_state(db, TARGET_ID, "awaiting_message")
        line_message_service.reply_message(reply_token, "送りたいメッセージを入力してください")
        return
        
    # 門限時刻の登録
    if data == "action=set_curfew":
        time_str = params.get("time") if params else None
        if time_str:
            # Targetテーブルに門限時刻を保存
            curfew_time = datetime.strptime(time_str, "%H:%M").time()
            db_service.update_curfew_time(db, TARGET_ID, curfew_time)
            
            # スケジューラのジョブを更新（既存ジョブを置き換え）
            scheduler.add_job(
                check_curfew_job,
                "cron",
                hour=curfew_time.hour,
                minute=curfew_time.minute,
                id=f"curfew_job_{TARGET_ID}",
                replace_existing=True
            )

            print("Scheduler job added.")
            # jobs = scheduler.get_jobs()
            # print("=== Registered Jobs ===")
            # for job in jobs:
            #     print(f"id: {job.id}, next_run_time: {job.next_run_time}, trigger: {job.trigger}")

            line_message_service.reply_message(reply_token, f"門限を {curfew_time} に登録しました！")

        else:
            line_message_service.reply_message(reply_token, "門限の時間が指定されませんでした。")
        return