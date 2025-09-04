from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import Target, TargetCreate


router = APIRouter()


@router.get("/targets")
def read_targets(db: Session = Depends(get_db)):
    return db.query(Target).all()


@router.post("/targets")
def create_target(target: TargetCreate, db: Session = Depends(get_db)):
    db_target = Target(**target.dict())
    db.add(db_target)
    db.commit()
    db.refresh(db_target)
    return db_target
