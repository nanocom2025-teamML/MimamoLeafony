# -*- coding: utf-8 -*-


from datetime import datetime, time
import os
from typing import Optional

from dotenv import load_dotenv

from fastapi import Depends, FastAPI

from pydantic import BaseModel

from sqlalchemy import create_engine
from sqlalchemy.orm import Session, sessionmaker

from models import Accesse, Base, Message, Target


# .env の読み込み
load_dotenv()


DATABASE_URL = os.getenv("DATABASE_URL", "sqlite:///./test.db")


engine = create_engine(
    DATABASE_URL, connect_args={"check_same_thread": False}
)


SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)


Base.metadata.create_all(bind=engine)


app = FastAPI()


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


class TargetCreate(BaseModel):
    name: str  # 必須入力
    curfew_time: Optional[time] = None  # 任意入力


class AccessCreate(BaseModel):
    target_id: int  # 必須入力
    gone_at: datetime  # 必須入力
    come_at: Optional[datetime] = None  # 任意入力


class MessageCreate(BaseModel):
    target_id: int  # 必須入力
    content: str  # 必須入力
    read_at: Optional[datetime] = None  # 任意入力


@app.get("/")
def read_root():
    return {"message": "Hello, MimamoLeafony!"}


@app.get("/targets/")
def read_targets(db: Session = Depends(get_db)):
    return db.query(Target).all()


@app.post("/targets/")
def create_target(target: TargetCreate, db: Session = Depends(get_db)):
    db_target = Target(**target.dict())
    db.add(db_target)
    db.commit()
    db.refresh(db_target)
    return db_target


@app.get("/accesses/")
def read_accesses(db: Session = Depends(get_db)):
    return db.query(Accesse).all()


@app.post("/accesses/")
def create_accesse(access: AccessCreate, db: Session = Depends(get_db)):
    db_accesse = Accesse(**access.dict())
    db.add(db_accesse)
    db.commit()
    db.refresh(db_accesse)
    return db_accesse


@app.get("/messages/")
def read_messages(db: Session = Depends(get_db)):
    return db.query(Message).all()


@app.post("/messages/")
def create_message(message: MessageCreate, db: Session = Depends(get_db)):
    db_message = Message(**message.dict())
    db.add(db_message)
    db.commit()
    db.refresh(db_message)
    return db_message
