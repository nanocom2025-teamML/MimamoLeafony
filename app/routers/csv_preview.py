import os, csv
from fastapi import APIRouter
from fastapi.responses import JSONResponse

router = APIRouter()

CSV_PATH = "./audio_files/mic_record.csv"


@router.get("/csv-preview")
def get_audio_preview(limit: int = 100):
    """
    CSV の中身（先頭10行）をレスポンスとして返す
    """
    if not os.path.exists(CSV_PATH):
        return JSONResponse(content={"error": "CSV file not found"}, status_code=404)

    try:
        with open(CSV_PATH, newline="") as f:
            reader = list(csv.reader(f))
            total_rows = len(reader)
            preview_rows = reader[:limit]

        return {
            "total_rows": total_rows,
            "preview": preview_rows
        }
    except Exception as e:
        return {"error": f"Failed to read CSV: {e}"}
