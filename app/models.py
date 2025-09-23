from datetime import datetime
from sqlalchemy import Boolean, Column, DateTime, ForeignKey, Integer, String, Text, Time, func, text, CheckConstraint
from .database import Base
from datetime import time
from typing import Optional
from pydantic import BaseModel
from app.services.jst import now_jst

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
    # 入力待ち状態（ENUM代替 → String + CheckConstraint）
    awaiting_state = Column(String(30), nullable=True)
    __table_args__ = (
        CheckConstraint(
            "awaiting_state IN ('awaiting_message','awaiting_curfew_time') OR awaiting_state IS NULL",
            name="check_awaiting_state"
        ),
    )
    # 登録日時
    created_at = Column(DateTime, default=now_jst)
    # 更新日時
    updated_at = Column(DateTime, default=now_jst, onupdate=now_jst)


class TargetCreate(BaseModel):
    name: str  # 必須入力
    curfew_time: Optional[time] = None  # 任意入力
    awaiting_state: Optional[str] = None  # 任意入力


class Access(Base):
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
    created_at = Column(DateTime, default=now_jst)
    # 更新日時
    updated_at = Column(DateTime, default=now_jst, onupdate=now_jst)


class AccessCreate(BaseModel):
    target_id: int  # 必須入力
    gone_at: datetime  # 必須入力
    come_at: Optional[datetime] = None  # 任意入力


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
    created_at = Column(DateTime, default=now_jst)
    # 更新日時
    updated_at = Column(DateTime, default=now_jst, onupdate=now_jst)


class MessageCreate(BaseModel):
    target_id: int  # 必須入力
    content: str  # 必須入力
    read_at: Optional[datetime] = None  # 任意入力
