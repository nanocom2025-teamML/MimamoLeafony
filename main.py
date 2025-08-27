# -*- coding: utf-8 -*-


from datetime import time
import os
from typing import Optional

from dotenv import load_dotenv

from fastapi import Depends, FastAPI

from pydantic import BaseModel

from sqlalchemy import Boolean, Column, DateTime, ForeignKey, Integer, String, Text, Time, create_engine, func, text
from sqlalchemy.orm import Session, declarative_base, sessionmaker


load_dotenv()


DATABASE_URL = os.getenv("DATABASE_URL", "sqlite:///./test.db")


engine = create_engine(
    DATABASE_URL, connect_args={"check_same_thread": False}
)


SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)


Base = declarative_base()


class Target(Base):
    """
    見守り対象者を表すモデルクラス
    """
    __tablename__ = "targets"

    # ターゲットID（PK）
    id = Column(Integer, primary_key=True, autoincrement=True)
    # ターゲット名
    name = Column(String(50), nullable=False)
    # 門限
    curfew_time = Column(Time, server_default=text("'20:00:00'"), nullable=True)
    # 登録日時
    created_at = Column(DateTime, server_default=func.now())
    # 更新日時
    updated_at = Column(DateTime, server_default=func.now(), onupdate=func.now())


class TargetCreate(BaseModel):
    name: str  # 必須
    curfew_time: Optional[time] = None  # 任意


class Accesse(Base):
    """
    外出・帰宅を管理するモデルクラス
    """
    __tablename__ = "accesses"

    # アクセスID（PK）
    id = Column(Integer, primary_key=True, autoincrement=True)
    # ターゲットID（FK）
    target_id = Column(Integer, ForeignKey('targets.id'), nullable=False)
    # 外出時刻
    gone_at = Column(DateTime, nullable=False)
    # 帰宅時刻（未帰宅時：NULL）
    come_at = Column(DateTime, nullable=True)
    # アラートの実行有無
    has_alerted = Column(Boolean, server_default=text("false"))
    # 登録日時
    created_at = Column(DateTime, server_default=func.now())
    # 更新日時
    updated_at = Column(DateTime, server_default=func.now(), onupdate=func.now())


class Message(Base):
    """
    メッセージを表すモデルクラス
    """
    __tablename__ = "messages"

    # メッセージID（PK）
    id = Column(Integer, primary_key=True, autoincrement=True)
    # ターゲットID（FK）
    target_id = Column(Integer, ForeignKey('targets.id'), nullable=False)
    # 本文
    content = Column(Text, nullable=False)
    # 受信日時（未受信時：NULL）
    read_at = Column(DateTime, nullable=True)
    # 登録日時
    created_at = Column(DateTime, server_default=func.now())
    # 更新日時
    updated_at = Column(DateTime, server_default=func.now(), onupdate=func.now())


Base.metadata.create_all(bind=engine)


app = FastAPI()


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


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
