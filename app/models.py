from sqlalchemy import Boolean, Column, DateTime, ForeignKey, Integer, String, Text, Time, func, text
from .database import Base
from datetime import time
from typing import Optional
from pydantic import BaseModel

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
