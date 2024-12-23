//
// Created by ddeut on 19.07.2023.
//

void emoji__process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case UKC_EMOJI_HERZ:
                if (record->event.pressed) {
                    send_unicode_string("\u2764\uFE0F");
                };
                break;
            case UKC_EMOJI_TRAURIG_STARK:
                if (record->event.pressed) {
                    send_unicode_string("\u2639\uFE0F");
                };
                break;
            case UKC_EMOJI_KEINE_AHNUNG:
                if (record->event.pressed) {
                    register_unicode(0x1F937);
                    send_unicode_string("\u200D\u2642\uFE0F");
                };
                break;
            case UKC_EMOJI_CHECK_MARK:
                if (record->event.pressed) {
                    send_unicode_string("\u2714\uFE0F");
                };
                break;
            case UKC_EMOJI_MELDEN:
                if (record->event.pressed) {
                    register_unicode(0x1F64B);
                    send_unicode_string("\u200D\u2642\uFE0F");
                };
                break;
            case UKC_EMOJI_ARROW_UP:
                if (record->event.pressed) {
                    send_unicode_string("\u2B06\uFE0F");
                };
                break;
            case UKC_EMOJI_ARROW_DOWN:
                if (record->event.pressed) {
                    send_unicode_string("\u2B07\uFE0F");
                };
                break;
            case UKC_EMOJI_ARROW_RIGHT:
                if (record->event.pressed) {
                    send_unicode_string("\u27A1\uFE0F");
                };
                break;
            case UKC_EMOJI_ARROW_LEFT:
                if (record->event.pressed) {
                    send_unicode_string("\u2B05\uFE0F");
                };
                break;
        }
    }
}