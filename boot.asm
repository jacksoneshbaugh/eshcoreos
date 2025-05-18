; eshcoreos bootloader
; &copy; Jackson Eshbaugh, 2025

BITS 16
ORG 0x7c00

; Constants for kernel loading
KERNEL_SEGMENT equ 0x1000   ; Segment to load kernel (0x1000:0 = 0x10000)
KERNEL_OFFSET  equ 0x0000   ; Offset within segment
KERNEL_SECTORS equ 2        ; Kernel size in sectors
START_SECTOR   equ 2        ; First sector of kernel

start:
  cli
  xor ax, ax
  mov ds, ax
  mov es, ax

  ; 1. Print message with BIOS
  mov si, msg
  call print_string

  ; 2. Load kernel into memory
  call load_kernel

  ; 3. Switch to protected mode

  ; Copy protected mode code to 0x90000
  mov si, protected_mode_code
  mov di, 0x9000
  mov es, di
  xor di, di
  mov cx, protected_mode_code_end - protected_mode_code
  rep movsb

  cli
  lgdt [gdt_descriptor] ; load global descriptor table

  ; enable protected mode
  mov eax, cr0
  or eax, 1
  mov cr0, eax

  jmp 0x08:init_pm ; far jump: flush instruction pipeline, switch segments

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 32-bit protected mode starts here ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

protected_mode_code:
[BITS 32]
init_pm:
  mov ax, 0x10         ; data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov esp, 0x90000
  jmp 0x08:0x10000 
.hang:
  hlt
  jmp .hang
protected_mode_code_end:

;;;;;;;;;;;;;;;;;;;;;
; BIOS PRINT STRING ;
;;;;;;;;;;;;;;;;;;;;;

[BITS 16]
print_string:
  .next_char:
    lodsb               ; load byte from DS:SI into AL
    or al, al           ; if AL == 0, end of string
    jz .done
    mov ah, 0x0e        ; bios teletype function
    int 0x10
    jmp .next_char
  .done:
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ENHANCED KERNEL LOADER     ;
; Loads multi-sector kernel  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

load_kernel:
    pusha                           ; Save all registers
    mov si, loading_msg
    call print_string

    ; Initialize segment, offset, and counters
    mov word [current_segment], KERNEL_SEGMENT
    mov cx, KERNEL_SECTORS          ; Total sectors to load

.load_loop:
    ; Check if we're done
    or cx, cx
    jz .load_complete
   
    ; Calculate sectors to read in this batch
    push cx                         ; Save remaining count
    cmp cx, 127                     ; Max sectors per read (127 is safe)
    jbe .no_size_adjust
    mov cx, 127                     ; If more than 127 sectors, read 127
.no_size_adjust:
    mov al, cl                      ; Sectors to read in AL

    ; Set up registers for INT 13h, function 02h
    mov ah, 0x02                    ; BIOS read sectors
    mov ch, [current_cylinder]      ; Cylinder number
    mov cl, [current_sector]        ; Sector number
    mov dh, [current_head]          ; Head number
    mov dl, 0x80                    ; Hard disk
    
    ; Set up destination
    mov bx, 0                       ; Offset in segment
    push es
    mov es, [current_segment]       ; Set segment

    int 0x13                        ; Call BIOS
    pop es                          ; Restore ES
    jc .disk_error                  ; Handle errors
    
    ; Update remaining sectors
    pop cx                          ; Restore remaining count
    sub cx, ax                      ; Subtract sectors read
    
    ; Update segment for next batch
    mov bx, ax                      ; Sectors read
    shl bx, 5                       ; Multiply by 32 (each sector is 512 bytes = 2^9)
                                    ; Shifting by 5 = 1/8 of a segment = 2048 bytes
    add [current_segment], bx       ; Add to segment

    ; Update CHS coordinates for next read
    add [current_sector], al        ; Add sectors read to current sector
    
    ; If we advanced to a new track, adjust head/cylinder
    ; For simplicity, assume 63 sectors per track, 255 heads
    cmp byte [current_sector], 63+1 ; Beyond sectors per track?
    jl .load_loop                   ; If not, continue
    
    mov byte [current_sector], 1    ; Reset to sector 1
    inc byte [current_head]         ; Increment head
    
    cmp byte [current_head], 255    ; Beyond heads per cylinder?
    jl .load_loop                   ; If not, continue
    
    mov byte [current_head], 0      ; Reset to head 0
    inc byte [current_cylinder]     ; Increment cylinder
    
    jmp .load_loop                  ; Continue loading

.disk_error:
    mov si, fail_msg
    call print_string
    jmp $                           ; Hang on error

.load_complete:
    mov si, success_msg
    call print_string
    popa                            ; Restore all registers
    ret

; Variables for kernel loading
current_segment   dw KERNEL_SEGMENT ; Current segment for loading
current_cylinder  db 0              ; Current cylinder
current_head      db 0              ; Current head
current_sector    db START_SECTOR   ; Start from sector 2

; Messages
msg          db "eshcoreos loading...", 13, 10, 0
loading_msg  db "Loading kernel... ", 0
success_msg  db "OK", 13, 10, 0
fail_msg     db "FAIL", 13, 10, 0

;;;;;;;;;;;;;;;;;;
; GDT Definition ;
;;;;;;;;;;;;;;;;;;

gdt_start:
  dq 0x0000000000000000    ; null descriptor
  dq 0x00CF9A000000FFFF    ; code segment
  dq 0x00CF92000000FFFF    ; data segment
gdt_end:

gdt_descriptor:
  dw gdt_end - gdt_start - 1
  dd gdt_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

; pad to 510 bytes + boot signature (512)
times 510 - ($ - $$) db 0
dw 0xaa55