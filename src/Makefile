.PHONY: calendar

calendar: calendar.h calendar.c
	gcc -std=c99 -pedantic -Wall -O3 calendar.c -o calendar

wasm: webp.c calendar.h calendar.c
	emcc -O3 -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' webp.c calendar.c
