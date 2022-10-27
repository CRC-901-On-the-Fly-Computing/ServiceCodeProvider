.phony: all clean

all:
	./gradlew classes
	./gradlew build
	cd cbuild && ./make_imagebridge.sh
	cd cbuild && ./make_pluginbridge.sh
	cd cbuild && ./make_services.sh

cpu_only:
	./gradlew classes
	./gradlew build
	cd cbuild && ./make_imagebridge.sh
	cd cbuild && ./make_pluginbridge.sh
	cd cbuild && ./make_services.sh -c

clean:
	cd cbuild && rm -rf *.so *.o
