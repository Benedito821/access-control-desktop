#!/bin/bash

set -e

IMAGE_NAME="access-control-admin-builder"
CONTAINER_NAME="access-control-admin-temp"
OUTPUT_DIR="./dist"
APPIMAGE_NAME="Access_Control_Admin-x86_64.AppImage"

echo "Building Docker image..."

docker build \
    -f Dockerfile.multistage \
    -t ${IMAGE_NAME} \
    .

echo "Creating temporary container..."

docker create --name ${CONTAINER_NAME} ${IMAGE_NAME}

mkdir -p ${OUTPUT_DIR}

echo "Extracting AppImage artifact..."

docker cp \
    ${CONTAINER_NAME}:/output/${APPIMAGE_NAME} \
    ${OUTPUT_DIR}/

echo "Cleaning up..."

docker rm ${CONTAINER_NAME}

echo "Done!"
echo "Artifact available at:"
echo "${OUTPUT_DIR}/${APPIMAGE_NAME}"
