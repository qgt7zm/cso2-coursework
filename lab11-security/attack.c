#include "lab.h"
#include <stdio.h>
#include <string.h>

/* forward function for "attack 0" case.
   change this code to implement your attack
 */
void forward_attack_0(struct message *message) {
    if (message->from == 'A') {
        // Change message text
        int amt = 10001438;
        snprintf((char *) message->data, sizeof(message->data), "PAY $%d TO M", amt / 2);
        message->data_size = strlen((char *) message->data);
    }

    send_message(message);
}

/* forward function for "attack 1" case.
   change this code to implement your attack
 */
void forward_attack_1(struct message *message) {
    if (message->from == 'A') {
        int amt = 10001438;
        char new_text[1024];
        snprintf(new_text, sizeof(new_text), "PAY $%d TO M", amt / 2);

        // Send different message
        struct message *new_msg = new_message(
            'A', 'B', new_text, 1, 0
        );
        send_message(new_msg);
    } else {
        send_message(message);
    }
}

/* forward function for "attack 2" case.
   change this code to implement your attack
 */
void forward_attack_2(struct message *message) {
    send_message(message);
}

/* forward function for "attack 3" case.
   change this code to implement your attack
 */
void forward_attack_3(struct message *message) {
    send_message(message);
}

/* forward function for "attack 4" case.
   change this code to implement your attack */
void forward_attack_4(struct message *message) {
    send_message(message);
}

/* forward function for "attack 5" case.
   I did not intend this one to be possible. */
void forward_attack_5(struct message *message) {
    send_message(message);
}
