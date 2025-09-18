from fastapi import FastAPI
from .database import Base, engine
from app.routers import webhook, accesses, device, messages, targets
from app.services.mqtt import run_mqtt

Base.metadata.create_all(bind=engine)

app = FastAPI()

app.include_router(webhook.router)
app.include_router(accesses.router)
app.include_router(device.router)
app.include_router(messages.router)
app.include_router(targets.router)

@app.on_event("startup")
def startup_event():
    run_mqtt()

@app.get("/")
def read_root():
    return {"message": "Hello, MimamoLeafony!"}
