FROM python:3.11-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

# Installiamo tutto nello stage finale: compilatori + runtime + vim
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    libffi-dev \
    libffi8 \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Installiamo i tool di build Python
RUN pip install --no-cache-dir scikit-build-core cffi

# Copiamo il resto dei file
COPY . .

CMD ["bash"]