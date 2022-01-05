# Docker
## Example
This Compose file defines two services: web and redis.<br>

The new volumes key mounts the project directory (current directory) on the host to /code inside the container, allowing you to modify the code on the fly, without having to rebuild the image.<br>
The environment key sets the FLASK_ENV environment variable, which tells flask run to run in development mode and reload the code on change.<br>
This mode should only be used in development.<br>

version: "3.9"<br>

**docker-compose.yml**
```Dockerfile
services:
  web:
    build: .
    ports:
      - "5000:5000"
    volumes:
      - .:/code
    environment:
      FLASK_ENV: development
  redis:
    image: "redis:alpine"
```

Build an image starting with the Python 3.7 image.<br>
Set the working directory to /code.<br>
Set environment variables used by the flask command.<br>
Install gcc and other dependencies.<br>
Copy requirements.txt and install the Python dependencies.<br>
Add metadata to the image to describe that the container is listening on port 5000.<br>
Copy the current directory . in the project to the workdir . in the image.<br>
Set the default command for the container to flask run.<br>

**Dockerfile**
```Dockerfile
# syntax=docker/dockerfile:1
FROM python:3.7-alpine
WORKDIR /code
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0
RUN apk add --no-cache gcc musl-dev linux-headers
COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt
EXPOSE 5000
COPY . .
CMD ["flask", "run"]
```

**app.py**
```python
import time

import redis
from flask import Flask

app = Flask(__name__)
cache = redis.Redis(host='redis', port=6379)

def get_hit_count():
    retries = 5
    while True:
        try:
            return cache.incr('hits')
        except redis.exceptions.ConnectionError as exc:
            if retries == 0:
                raise exc
            retries -= 1
            time.sleep(0.5)

@app.route('/')
def hello():
    count = get_hit_count()
    return 'Hello World! I have been seen {} times.\n'.format(count)
```

**requirements.txt**
```
flask
redis
```

## Commands
Run above application
```bash
$ docker-compose up
$ docker image ls
```

Run background
```bash
$ docker-compose up -d
$ docker-compose ps
```

Stop all services
```bash
$ docker-compose stop
```

Example
```bash
$ docker pull busybox
$ docker images
$ docker run busybox echo "Hello world"
$ docker ps
$ docker run -it busybox sh
```

Delete exited containers
```bash
$ docker rm [CONTAINER_ID]
```

Delete all exited containers
```bash
$ docker rm $(docker ps -a -q -f status=exited)
    or
$ docker container prune
```