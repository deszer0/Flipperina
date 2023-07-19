#include <stdio.h>
#include <stdbool.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>

#define NOTE_UP 587.33f
#define NOTE_LEFT 493.88f
#define NOTE_RIGHT 440.00f
#define NOTE_DOWN 349.23
#define NOTE_OK 293.66f

typedef struct {
    FuriMutex* model_mutex;

    FuriMessageQueue* event_queue;

    ViewPort* view_port;
    Gui* gui;
} Ocarina;

void draw_callback(Canvas* canvas, void* ctx) {
    Ocarina* ocarina = ctx;
    furi_check(furi_mutex_acquire(ocarina->model_mutex, FuriWaitForever) == FuriStatusOk);

    canvas_draw_frame(canvas, 0, 0, 128, 64);
    canvas_draw_str(canvas, 50, 10, "Ocarina");
    canvas_draw_str(canvas, 30, 20, "OK button for A");

    furi_mutex_release(ocarina->model_mutex);
}

void input_callback(InputEvent* input, void* ctx) {
    Ocarina* ocarina = ctx;
    furi_message_queue_put(ocarina->event_queue, input, FuriWaitForever);
}

int targetSequence[] = {NOTE_UP, NOTE_LEFT, NOTE_RIGHT, NOTE_DOWN};
int targetLength = sizeof(targetSequence) / sizeof(targetSequence[0]);

bool compareSequences(int seq1[], int seq2[], int length) {
    for (int i = 0; i < length; i++) {
        if (seq1[i] != seq2[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    int userInput;
    int userSequence[4];
    int currentIndex = 0;

    while (1) {

        userSequence[currentIndex] = userInput;
        currentIndex++;

        if (currentIndex >= targetLength) {
            if (compareSequences(userSequence, targetSequence, targetLength)) {
                currentIndex = 0;
                for (int i = 0; i < targetLength; i++) {
                    userSequence[i] = 0;
                }
              
            } else {
                currentIndex = 0;
                for (int i = 0; i < targetLength; i++) {
                    userSequence[i] = 0;
                }
            }
        }
    }

    return 0;
}
