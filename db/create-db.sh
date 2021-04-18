#!/bin/sh

set -ue

if command -v podman > /dev/null 2>&1; then
        CONTAINER_ENGINE=podman
elif command -v docker > /dev/null 2>&1; then
        CONTAINER_ENGINE=docker
else
        echo "No container engine found!!!"
        exit 1
fi

# Permanent storage
"${CONTAINER_ENGINE}" run -ti \
    --name scrapod-db \
    -e POSTGRES_PASSWORD="${POSTGRES_PASSWORD}" \
    -p "5432:5432" \
    -v "${PWD}/init-db.d:/docker-entrypoint-initdb.d" \
    postgres
