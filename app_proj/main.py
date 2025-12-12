try:
    import clustering
except ImportError as e:
    print("ERROR: Build clustering first!!!!, see README.md")
    raise e

from typing import List, Dict, Any, Optional

from fastapi import FastAPI, Request, HTTPException
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from fastapi.staticfiles import StaticFiles

from pydantic import BaseModel
from scipy.cluster.hierarchy import fcluster

from clustering import HClustering, MetricType, LinkageType, DataPoint

app = FastAPI()
app.mount("/static", StaticFiles(directory="static"), name="static")
templates = Jinja2Templates(directory='templates')

class RowData(BaseModel):
    row_id: int
    data_vector: List[float]
    row_class: Optional[int]

    def form_dict(self) -> Dict[str, Any]:
        return {
                'row_id': self.row_id,
                'data_vector': self.data_vector,
                'row_class': self.row_class,
        }

class SaveData(BaseModel):
    rows: List[RowData]


test_data = [
    [0., 0.], [0., 1.], [1., 0.],
    [0., 4.], [0., 3.], [1., 4.],
    [4., 0.], [3., 0.], [4., 1.],
    [4., 4.], [3., 4.], [4., 3.],
]

test_data_points = [RowData(row_id=i, data_vector=test_data[i], row_class=None) for i in range(len(test_data))]
# test_data_row_1 = RowData(row_id=0, data_vector=[0, 1], row_class=None)
# test_data_row_2 = RowData(row_id=1, data_vector=[1, 1], row_class=None)
data: SaveData = SaveData(rows=test_data_points)

@app.get("/", response_class=HTMLResponse)
async def index(request: Request):
    return templates.TemplateResponse("index.html", {"request": request })

@app.get('/data')
async def get_data():
    return [now_row.form_dict() for now_row in data.rows]

class UpdateCellRequest(BaseModel):
    row_id: int
    col_id: int
    new_val: float

@app.post('/update_cell')
async def update_cell(request: UpdateCellRequest):
    row_id = request.row_id
    col_id = request.col_id
    new_data = request.new_val

    if row_id >= len(data.rows):
        raise HTTPException(status_code=400, detail="Trying to save data for unknown row")

    if col_id >= len(data.rows[0].data_vector):
        raise HTTPException(status_code=400, detail="Trying to save data for unknown col")

    data.rows[row_id].data_vector[col_id] = new_data
    print(f"New data set for ROW:{row_id}; COL:{col_id}; DATA:{new_data}")

@app.post('/add_row')
async def add_row():
    new_id = len(data.rows)
    dims = len(data.rows[0].data_vector)
    new_data = [0.0 for _ in range(dims)]
    new_data_row = RowData(row_id=new_id, data_vector=new_data, row_class=None);
    data.rows.append(new_data_row)

@app.post('/add_col')
async def add_col():
    for now_row in data.rows:
        now_row.data_vector.append(0.0)

class ClassificationParams(BaseModel):
    max_clusters: Optional[int]
    distance: Optional[float]

@app.post('/classify')
async def classify(request: ClassificationParams):
    max_clusters = request.max_clusters
    distance = request.distance
    assert max_clusters is not None or distance is not None

    points_data = [DataPoint(now_row.data_vector) for now_row in data.rows]

    model = HClustering(
        dataset = points_data,
        metric = MetricType.EUCLIDEAN,
        linkage = LinkageType.WPGMA
    )
    result = model.fit()

    if max_clusters is not None:
        labels = fcluster(result, max_clusters, criterion="maxclust")
    else:
        labels = fcluster(result, distance, criterion="distance")

    labels = list(map(int, labels))

    for (now_row_id, now_row) in enumerate(data.rows):
        now_row.row_class = labels[now_row_id]
