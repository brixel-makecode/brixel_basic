//% color="#F1C40F" icon="\uf108" block="OLED(SSD1306) 한글"
namespace Han_OLED_SSD1306 {

    // I2C Address
    const OLED_ADDR = 0x3c;
    const OLED_WIDTH = 128;
    const OLED_HEIGHT = 64;

    // Hangeul Composition Tables
    const cho1 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0];
    const cho2 = [0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5];
    const jong = [0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1];

    export class Han_OLED_Class {

        constructor() {
        }

        private command_write(command: number) {
            let buf = pins.createBuffer(2);
            buf[0] = 0x00;
            buf[1] = command;
            pins.i2cWriteBuffer(OLED_ADDR, buf);
        }

        private data_write(data: number) {
            let buf = pins.createBuffer(2);
            buf[0] = 0x40;
            buf[1] = data;
            pins.i2cWriteBuffer(OLED_ADDR, buf);
        }

        public init(): void {
            // Initialization Sequence
            let init_seq = [
                0xAE, 0x20, 0x00, 0xB0, 0xC8, 0x00, 0x10, 0x40,
                0x81, 0x3F, 0xA1, 0xA6, 0xA8, 0x3F, 0xA4, 0xD3,
                0x00, 0xD5, 0xF0, 0xD9, 0x22, 0xDA, 0x12, 0xDB,
                0x20, 0x8D, 0x14, 0xAF
            ];

            for (let i = 0; i < init_seq.length; i++) {
                this.command_write(init_seq[i]);
            }
            this.clearDisplay();
        }

        public clearDisplay(): void {
            for (let i = 0; i < 8; i++) {
                this.gotoXY(0, i);
                let buf = pins.createBuffer(OLED_WIDTH + 1);
                buf[0] = 0x40; // Data mode
                pins.i2cWriteBuffer(OLED_ADDR, buf);
            }
        }

        private gotoXY(x: number, y: number) {
            this.command_write(0xb0 + y);
            this.command_write(0x00 + (8 * x & 0x0f));
            this.command_write(0x10 + ((8 * x >> 4) & 0x0f));
        }

        public printString(str: string, x: number, y: number, inverse: number): void {
            let i = 0;
            while (i < str.length) {
                let code = str.charCodeAt(i);
                if (code < 0x80) {
                    this.putchEng(x, y, code, inverse == 0);
                    x++;
                    i++;
                } else {
                    this.putchKor(x, y, str.charCodeAt(i), inverse == 0);
                    x += 2;
                    i++;
                }
            }
        }

        private putchEng(x: number, y: number, ch: number, normal: boolean) {
            // Safety check for font data
            if (typeof E_font === 'undefined') return;

            this.gotoXY(x, y);
            let buf = pins.createBuffer(9);
            buf[0] = 0x40;
            for (let i = 0; i < 8; i++) {
                let val = E_font[ch * 16 + i];
                buf[i + 1] = normal ? val : ~val;
            }
            pins.i2cWriteBuffer(OLED_ADDR, buf);

            this.gotoXY(x, y + 1);
            for (let i = 0; i < 8; i++) {
                let val = E_font[ch * 16 + i + 8];
                buf[i + 1] = normal ? val : ~val;
            }
            pins.i2cWriteBuffer(OLED_ADDR, buf);
        }

        private putchKor(x: number, y: number, unicode: number, normal: boolean) {
            // Safety check for font data
            if (typeof K_font === 'undefined') return;

            // Decompose Unicode
            unicode -= 0xAC00;
            let last = unicode % 28;
            unicode = Math.idiv(unicode, 28);
            let first = Math.idiv(unicode, 21) + 1;
            let mid = unicode % 21 + 1;

            let firstType = 0;
            let midType = 0;
            let lastType = 0;

            if (last == 0) {
                firstType = cho1[mid];
                if (first == 1 || first == 24) midType = 0;
                else midType = 1;
            } else {
                firstType = cho2[mid];
                if (first == 1 || first == 24) midType = 2;
                else midType = 3;
                lastType = jong[mid];
            }

            let buf = pins.createBuffer(32);

            // First (Cho)
            let pF_temp = firstType * 20 + first;
            for (let i = 0; i < 32; i++) {
                buf[i] = K_font[pF_temp * 32 + i];
            }

            // Middle (Jung)
            pF_temp = 160 + midType * 22 + mid;
            for (let i = 0; i < 32; i++) {
                buf[i] |= K_font[pF_temp * 32 + i];
            }

            // Last (Jong)
            if (last) {
                pF_temp = 248 + lastType * 28 + last;
                for (let i = 0; i < 32; i++) {
                    buf[i] |= K_font[pF_temp * 32 + i];
                }
            }

            // Draw Top Half
            this.gotoXY(x, y);
            let wireBuf = pins.createBuffer(17);
            wireBuf[0] = 0x40;
            for (let i = 0; i < 16; i++) {
                wireBuf[i + 1] = normal ? buf[i] : ~buf[i];
                if (!normal) wireBuf[i + 1] &= 0xFF;
            }
            pins.i2cWriteBuffer(OLED_ADDR, wireBuf);

            // Draw Bottom Half
            this.gotoXY(x, y + 1);
            for (let i = 0; i < 16; i++) {
                wireBuf[i + 1] = normal ? buf[i + 16] : ~buf[i + 16];
                if (!normal) wireBuf[i + 1] &= 0xFF;
            }
            pins.i2cWriteBuffer(OLED_ADDR, wireBuf);
        }

        public showBigNumber(num: number, x: number, y: number, inverse: number): void {
            if (typeof Seg_font === 'undefined') return;

            let normal = inverse == 0;

            for (let i = 0; i < 4; i++) {
                this.gotoXY(x, y + i);
                let wireBuf = pins.createBuffer(28);
                wireBuf[0] = 0x40;

                for (let j = 0; j < 27; j++) {
                    let srcIndex = num * 128 + i * 32 + (j + 3);
                    let val = Seg_font[srcIndex];
                    wireBuf[j + 1] = normal ? val : ~val;
                    if (!normal) wireBuf[j + 1] &= 0xFF;
                }
                pins.i2cWriteBuffer(OLED_ADDR, wireBuf);
            }
        }

        public drawBarGraph(x: number, y: number, value: number): void {
            this.gotoXY(x, y);

            let buf = pins.createBuffer(129); // 1 command + 128 data
            buf[0] = 0x40;

            for (let k = 0; k < 128; k++) {
                if (k % 2 == 0) {
                    buf[k + 1] = 0;
                } else {
                    if (k <= value) buf[k + 1] = 0x7e;
                    else buf[k + 1] = 0;
                }
            }
            pins.i2cWriteBuffer(OLED_ADDR, buf);
        }
    }

    // Singleton instance
    let _oled: Han_OLED_Class;
    function getOLED(): Han_OLED_Class {
        if (!_oled) _oled = new Han_OLED_Class();
        return _oled;
    }

    // Exported Blocks

    /**
     * OLED(SSD1306) 한글 디스플레이 설정
     */
    //% blockId=oled_init_v2 block="OLED(SSD1306) 한글 디스플레이 설정"
    //% weight=100
    export function init(): void {
        getOLED().init();
    }

    /**
     * OLED 화면 지우기
     */
    //% blockId=oled_clear_v2 block="OLED 화면 지우기"
    //% weight=90
    export function clearDisplay(): void {
        getOLED().clearDisplay();
    }

    /**
     * 텍스트 출력
     */
    //% blockId=oled_print_string_v2 block="텍스트 출력 %str 위치 X %x Y %y 반전 %inverse"
    //% inverse.defl=0
    //% inlineInputMode=inline
    //% weight=80
    export function printString(str: string, x: number, y: number, inverse: number): void {
        getOLED().printString(str, x, y, inverse);
    }

    /**
     * 큰 숫자 표시
     */
    //% blockId=oled_big_number_v2 block="큰 숫자 표시 %num 위치 X %x Y %y 반전 %inverse"
    //% inverse.defl=0
    //% inlineInputMode=inline
    //% weight=70
    export function showBigNumber(num: number, x: number, y: number, inverse: number): void {
        getOLED().showBigNumber(num, x, y, inverse);
    }

    /**
     * 막대 그래프 그리기
     */
    //% blockId=oled_draw_bar_v2 block="막대 그래프 그리기 X %x Y %y 값 %value"
    //% weight=60
    export function drawBarGraph(x: number, y: number, value: number): void {
        getOLED().drawBarGraph(x, y, value);
    }
}
