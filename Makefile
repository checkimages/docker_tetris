.PHONY: all build run dockerhub clean_containers clean_all_images clean

TARGET := docker_tetris
DOCKERHUB := checkimages/tetris:latest
#-------------------------------------------------------------

all: build run

build:clean
	docker build -t $(TARGET) .

run:
	docker run -it --rm -v .:/project --device /dev/tty $(TARGET)
#	docker run -it --rm -v .:/project --device /dev/tty -e TCOLOR=1 -e TBRIGHT=1 $(TARGET)

dockerhub:
	docker tag $(TARGET) $(DOCKERHUB)
	docker push $(DOCKERHUB)

clean_containers: 
	if [ ! -z "$$(docker ps -a -q)" ]; then docker stop $$(docker ps -a -q); docker rm $$(docker ps -a -q); fi
	if [ ! -z "$$(docker images -q $(TARGET))" ]; then docker rmi $(TARGET); fi

clean_all_images:
	if [ ! -z "$$(docker images -q)" ]; then docker rmi $$(docker images -q); fi

clean: clean_containers