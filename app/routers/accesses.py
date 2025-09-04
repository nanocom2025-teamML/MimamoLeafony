from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import Access, AccessCreate


router = APIRouter()


@router.get("/accesses")
def read_accesses(db: Session = Depends(get_db)):
    return db.query(Access).all()


@router.post("/accesses")
def create_access(access: AccessCreate, db: Session = Depends(get_db)):
    db_access = Access(**access.dict())
    db.add(db_access)
    db.commit()
    db.refresh(db_access)
    return db_access
