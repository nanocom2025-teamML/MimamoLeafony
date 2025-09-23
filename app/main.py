from fastapi import FastAPI
from contextlib import asynccontextmanager
from app.database import Base, engine, SessionLocal
from app.routers import webhook, accesses, device, messages, targets, csv_preview
# from app.services.mqtt import run_mqtt, stop_mqtt
from app.jobs.curfew_job import start_scheduler, scheduler
from app.services.db_service import create_target
from app import models
from app.config import TARGET_ID

Base.metadata.create_all(bind=engine)

@asynccontextmanager
async def lifespan(app: FastAPI):
    # 起動時
    # run_mqtt()
    db = SessionLocal()
    try:
        # id=1 のデータが存在しなければ作成
        if not db.query(models.Target).filter_by(id=TARGET_ID).first():
            create_target(db, name="aomi", target_id=TARGET_ID)
            print(f"Seed target created: id={TARGET_ID}, name=aomi")

        if not scheduler.running:
            start_scheduler()
            print("Scheduler started")
    finally:
        db.close()

    yield
    # 終了時
    if scheduler.running:
        scheduler.shutdown(wait=False)
        print("Scheduler stopped")
        # stop_mqtt()

app = FastAPI(lifespan=lifespan)

app.include_router(webhook.router)
app.include_router(accesses.router)
app.include_router(device.router)
app.include_router(messages.router)
app.include_router(targets.router)
app.include_router(csv_preview.router)


@app.get("/")
def read_root():
    return {"message": "Hello, MimamoLeafony!"}
