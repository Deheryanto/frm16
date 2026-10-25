; Program utama: idle
LOADI R1, 0     ; 0x0000
LOOP_UTAMA:
    JMP LOOP_UTAMA  ; 0x0002

; Rutin interrupt di 0x0004
ADDI R1, 1      ; 0x0004
; Gambar titik di (R1, 64)
LOADI8 64
MOV R2, R7      ; Y = 64
MOV R3, R1      ; X = R1
LOADI R4, 1     ; Warna putih
MOV R1, R4      ; R1 = warna (untuk DRAWR)
DRAWR R3, R2    ; Gambar di (X, Y)
MOV R1, R3      ; Kembalikan R1 (counter)
IRET            ; 0x000A (kira-kira)