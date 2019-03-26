all: flickr_interesting_photo
	

flickr_interesting_photo: main.cpp photo_with_tag_not_found_error.cpp interesting_photo_service.cpp interesting_photo.cpp
	g++ $^ -lcpprest -lpthread -lssl -lcrypto -lboost_system -O3 -o $@

