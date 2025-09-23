from apscheduler.schedulers.background import BackgroundScheduler
from datetime import datetime
from app.database import SessionLocal
from app.services import db_service, line_message_service
from app.config import TARGET_ID, LINE_USER_ID
from app.services.jst import JST, now_jst
from app.services.db_service import create_target

scheduler = BackgroundScheduler(timezone=JST)

def check_curfew_job():
    """
    門限チェックジョブ
    """
    print("=== Checking curfew... ===")
    db = SessionLocal()
    try:
        curfew_time = db_service.get_curfew_time(db, TARGET_ID)
        print(f"curfew_time from DB: {curfew_time}")
        if not curfew_time:
            print("門限未設定、処理終了")
            return

        now = now_jst().time()
        print(f"current time: {now}")

        # 門限超過チェック
        if now >= curfew_time:
            print("門限超過判定: True")
            # 通知対象となる外出レコードを取得
            access = db_service.get_unalerted_outside_access(db, TARGET_ID)
            print(f"access fetched: {access}")
            if access:
                # LINE通知
                line_message_service.push_confirm_template(
                    user_id=LINE_USER_ID,
                    text="門限を過ぎています。すでに帰宅していますか？"
                )
                print(f"[{now}] curfew alert sent for access_id={access.id}")

                # 通知済みに更新
                db_service.update_has_alerted(db, access.id)

                # awaiting_curfew_time状態に更新
                db_service.update_awaiting_state(db, access.target_id, state="awaiting_curfew_time")
                print(f"access_id={access.id} marked as alerted")
            else:
                print("通知対象の外出レコードなし")
        else:
            print("門限超過判定: False")
    finally:
        db.close()


def start_scheduler():
    db = SessionLocal()
    try:
        curfew_time: datetime.time = db_service.get_curfew_time(db, TARGET_ID)
        if not curfew_time:
            print("curfew_time 未設定")
            create_target(db, name="kaihey", target_id=TARGET_ID)
            print(f"Seed target created: id={TARGET_ID}, name=kaihey")

        scheduler.add_job(
            check_curfew_job,
            "cron",
            hour=curfew_time.hour,
            minute=curfew_time.minute,
            id=f"curfew_job_{TARGET_ID}",
            replace_existing=True
        )
        scheduler.start()
        print(f"Scheduler started with curfew_time={curfew_time}")
    finally:
        db.close()

