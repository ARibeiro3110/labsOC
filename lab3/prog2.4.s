                .data
A:              .word   1, 3, 1, 6, 4
                .word   2, 4, 3, 9, 5
mult:   .word   0

        .code
        daddi   $1, $0, A       ; *A[0]
        daddi   $5, $0, 1       ; $5 = 1            ;; i = 1
        daddi   $6, $0, 7       ; $6 = N            ;; N = 7
        lw      $9, 0($1)       ; $9 = A[0]         ;; mult = A[0]
        lw      $10, 8($1)      ; $10 = A[1]
        lw      $11, 16($1)     ; $11 = A[2]

loop:   dmul    $20, $10, $9    ; $20 = $10*$9      ;; $20 = A[i]*mult
        daddi   $1, $1, 16      ;
        lw      $10, 8($1)      ; $10 = A[i+2]
        dadd    $9, $9, $20     ; $9 = $9 + $20     ;; mult = mult + A[i]*mult

        dmul    $21, $11, $9    ; $21 = $11*$9      ;; $21 = A[i+1]*mult
        lw      $11, 16($1)     ; $11 = A[i+3]
        daddi   $5, $5, 2       ; i += 2
        dadd    $9, $9, $21     ; $9 = $9 + $21     ;; mult = mult + A[i+1]*mult

        bne     $6, $5, loop    ; Exit loop if i == 7

        dmul    $20, $10, $9    ; $20 = $10*$9      ;; $20 = A[i]*mult
        lw      $12, 24($1)     ; $12 = A[i+2]
        dadd    $9, $9, $20     ; $9 = $9 + $20     ;; mult = mult + A[i]*mult

        dmul    $21, $11, $9    ; $21 = $11*$9      ;; $21 = A[i+1]*mult
        dadd    $9, $9, $21     ; $9 = $9 + $21     ;; mult = mult + A[i+1]*mult

        dmul    $22, $12, $9    ; $22 = $12*$9      ;; $22 = A[i+2]*mult
        dadd    $9, $9, $22     ; $9 = $9 + $22     ;; mult = mult + A[i+2]*mult

        sw      $9, mult($0)    ; Store result
        halt

;; Expected result: mult = f6180 (hex), 1008000 (dec)