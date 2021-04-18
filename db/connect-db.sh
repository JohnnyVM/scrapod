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
"${CONTAINER_ENGINE}" exec -u postgres -ti \
	scrapod-db \
    psql -U scrapod -d scrapod
