g++ -DDBUS_API_SUBJECT_TO_CHANGE `pkg-config --libs --cflags packagekit-glib2` PackageKitManager.cpp main.cpp -o package-kit
