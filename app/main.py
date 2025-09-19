from fastapi import FastAPI
from contextlib import asynccontextmanager
from .database import Base, engine
from app.routers import webhook, accesses, device, messages, targets
from app.services.mqtt import run_mqtt, stop_mqtt
from app.jobs.curfew_job import start_scheduler, scheduler

Base.metadata.create_all(bind=engine)

@asynccontextmanager
async def lifespan(app: FastAPI):
    # 起動時
    run_mqtt()
    
    if not scheduler.running:
        start_scheduler()
        print("Scheduler started")
    yield
    # 終了時
    if scheduler.running:
        scheduler.shutdown(wait=False)
        print("Scheduler stopped")
        stop_mqtt()

app = FastAPI(lifespan=lifespan)

app.include_router(webhook.router)
app.include_router(accesses.router)
app.include_router(device.router)
app.include_router(messages.router)
app.include_router(targets.router)


@app.get("/")
def read_root():
    return {"message": "Hello, MimamoLeafony!"}
