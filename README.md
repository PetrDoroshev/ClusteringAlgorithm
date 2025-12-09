## Build clustering
1. ```mkdir -p build```
2. ```cd build```
3. ```cmake ..```
4. ```make```

## Run app
1. ```cd app_proj```
2. ```source .venv/bin/activate```
3. ```uvicorn main:app --reload```
4. Go to http://127.0.0.1:8000
