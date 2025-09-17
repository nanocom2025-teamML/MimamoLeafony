from sqlalchemy.orm import Session
from app import models
from datetime import datetime, time

def create_message(db: Session, target_id: int, text: str):
    """新しいメッセージを作成"""
    message = models.Message(target_id=target_id, content=text)
    db.add(message)
    db.commit()
    db.refresh(message)
    return message


def get_curfew_time(db: Session, target_id: int) -> time:
    """門限を取得"""
    target = db.query(models.Target).filter(models.Target.id == target_id).first()
    if not target or not target.curfew_time:
        return None

    return target.curfew_time


def update_curfew_time(db: Session, target_id: int, time_str: str):
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


def update_has_alerted(db: Session, access_id: int) -> None:
    """対象者の has_alerted を更新"""
    access = db.query(models.Access).filter(models.Access.id == access_id).first()
    if not access:
        return

    access.has_alerted = True
    db.commit()
    db.refresh(access)


def get_unalerted_outside_access(db: Session, target_id: int):
    """未通知の外出中のアクセスを取得"""
    return (
        db.query(models.Access)
        .filter(
            models.Access.target_id == target_id,
            models.Access.has_alerted == False,
            models.Access.come_at == None
        )
        .order_by(models.Access.gone_at.desc())
        .first()
    )


def update_curfew_return(db: Session, target_id: int) -> bool:
    """
    門限通知済みで未帰宅の最新レコードを探し、
    見つかれば come_at を現在時刻で更新する。
    戻り値: 更新できたら True, 見つからなければ False
    """
    access = (
        db.query(models.Access)
        .filter(
            models.Access.target_id == target_id,
            models.Access.come_at == None,
            models.Access.has_alerted == True,
        )
        .order_by(models.Access.gone_at.desc())
        .first()
    )

    if not access:
        return False

    access.come_at = datetime.now()
    db.commit()
    db.refresh(access)
    return True
