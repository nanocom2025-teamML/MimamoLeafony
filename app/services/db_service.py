from sqlalchemy.orm import Session
from app import models

def create_message(db: Session, target_id: int, text: str):
    """新しいメッセージを作成"""
    message = models.Message(target_id=target_id, content=text)
    db.add(message)
    db.commit()
    db.refresh(message)
    return message


def update_curfew(db: Session, target_id: int, time_str: str):
    """門限を更新（存在しなければ作成）"""
    target = db.query(models.Target).filter(models.Target.id == target_id).first()
    if target:
        target.curfew_time = time_str
    else:
        # 新規ターゲット作成（必要なら）
        target = models.Target(id=target_id, curfew_time=time_str)
        db.add(target)
    db.commit()
    db.refresh(target)
    return target


def get_curfew(db: Session, target_id: int):
    """門限を取得"""
    return db.query(models.Target).filter(models.Target.id == target_id).first().curfew_time


def get_awaiting_state(db: Session, target_id: int) -> str | None:
    """対象者の awaiting_state を取得"""
    return db.query(models.Target).filter(models.Target.id == target_id).first().awaiting_state


def update_awaiting_state(db: Session, target_id: int, state: str | None) -> None:
    """対象者の awaiting_state を更新"""
    target = db.query(models.Target).filter(models.Target.id == target_id).first()
    if not target:
        return

    target.awaiting_state = state
    db.commit()
    db.refresh(target)