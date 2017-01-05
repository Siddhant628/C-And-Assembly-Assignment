; A program which takes a section number as an input to estimate the averages of the
; bytes, words and longs present in the specific section of data stored in file "OutputBinaryFile.txt"

DISPLAY_STRING_TRAP                 EQU 13
DISPLAY_STRING_WITHOUT_CRLF_TRAP    EQU 14
READ_NUMBER_FROM_KEYBOARD_TRAP      EQU  4
DISPLAY_NUMBER_TRAP                 EQU  3


START   ORG    $1000
; Prompt use for entering the section number
    lea getUserInput, a1
    move.l  #DISPLAY_STRING_TRAP, d0
    trap    #15
; Get the user number input    
    move.l  #READ_NUMBER_FROM_KEYBOARD_TRAP, d0
    trap    #15
; Check if the user input is valid
    move.l  inputBinaryFile, d0
    cmp.l   d0, d1
    bgt     stopExecution
; Initialize the nummber of padding bytes to 0
    move.b  #0, numberOfPaddingBytes
; Jump to the section whose data needs to be checked
    move.l  #inputBinaryFile, a1
    add.l   #4, a1                          ; Move to the first section
loopToData:
    sub.l   #1, d1                          ; D1 is a counter to approach the section
    beq     sectionReached                  ; Check if section has been reached
    clr     d0
    
    move.w  (a1), d0    
    add.l   d0, a1
    add.l   #8, a1                          ; 4 Words are taken by the number of net number of bytes, bytes, words and longs of each section
    bra loopToData
; a1 points at the first byte of the section
sectionReached:
    add.l   #2, a1                          ; Skip the word containing net number of bytes in this section
; Check for padding byte in the chunk of bytes
    move.l  a1, a2
    add.w   (a1), a2                        ; Add the number of bytes to reach the padding byte
    add.l   #5, a2                          ; Offset to padding byte due to 4 bytes taken by number of words and longs in data
    move.b  (a2), d4
    cmpi.l  #0, d4                          ; If byte = 0, then padding exists
    bne     continueWithoutPadding
    jsr     paddingByteExists

continueWithoutPadding:   
    move.w  (a1)+, numBytes
    move.w  (a1)+, numShorts
    move.w  (a1)+, numLongs
; a1 now points at the first byte of data in the current section   
; Perform averaging operations
    clr     d0
    clr     d2                              ; Used as counter for bytes
    clr     d1                              ; Used as variable to add the sum of bytes
    clr     d5                              ; Used to store the number of padding bytes
    move.w  numBytes, d2
    move.b  numberOfPaddingBytes, d5
    sub.l   d5, d2
; Loop for estimating the number of bytes
loopForBytes:
    move.b  (a1)+, d0
    add.l   d0, d1
    sub.l   #1, d2
    bne     loopForBytes
    
    clr     d0
    move.w  numBytes, d0
    sub.l   d5, d0              ; D5 has the number of padding bytes
    divu    d0, d1              ; The result of division for average operation
; Rouding for byte average
    move.l  a1, a2              ; Save first address of words to be averaged for refering later
    clr     d0
    move.w  numBytes, d0
    sub.l   d5, d0              ; Number of Bytes = Total bytes - Number of padding bytes
    move.l  d1, d2
    andi.l  #$0000FFFF, d1      ; D1 contains the dividend
    andi.l  #$FFFF0000, d2
    ror.l   #8, d2
    ror.l   #8, d2              ; D2 contains remainder
    move.l  d0, d3
    divu    #2, d3
    and.l   #$0000FFFF, d3      ; D3 has half of the divisor
    mulu    #100, d2
    add.l   d3, d2
    divu    d0, d2
    andi.l  #$0000FFFF, d2      ; D2 Contains the decimal term
    jsr     checkForProperRounding
; Displaying the average value
    lea averageByte, a1
    move.l  #DISPLAY_STRING_WITHOUT_CRLF_TRAP, d0
    trap    #15 
    jsr     displayNumber
; Add the number of padding bytes to address to reach the chunk of words
    add.l   d5, a2   
; Average operation for words
    clr     d0 
    clr     d2                   ; Used as counter for words
    clr     d1                   ; Used as variable to add the sum of bytes
    move.w  numShorts, d2       
loopForWords:
    move.w  (a2)+, d0
    add.l   d0, d1
    sub.l   #1, d2
    bne     loopForWords    
 
    clr     d0
    move.w  numShorts, d0
    divu    d0, d1
; Rouding for word average 
    clr     d0
    move.w  numShorts, d0
    move.l  d1, d2
    andi.l  #$0000FFFF, d1      ; D1 contains the dividend
    andi.l  #$FFFF0000, d2
    ror.l   #8, d2
    ror.l   #8, d2              ; D2 contains remainder
    move.l  d0, d3
    divu    #2, d3
    and.l   #$0000FFFF, d3      ; D3 has half of the divisor
    mulu    #100, d2
    add.l   d3, d2
    divu    d0, d2
    andi.l  #$0000FFFF, d2      ; D2 Contains the decimal term
    jsr     checkForProperRounding
; Displaying the average value
    lea averageShort, a1
    move.l  #DISPLAY_STRING_WITHOUT_CRLF_TRAP, d0
    trap    #15 
    jsr     displayNumber
; Average operation for longs
    clr     d2                   ; Used as counter for words
    clr     d1                   ; Used as variable to add the sum of bytes
    move.w  numLongs, d2       
loopForLongs:
    add.l   (a2)+, d1
    sub.l   #1, d2
    bne     loopForLongs    
 
    clr     d0
    move.w  numLongs, d0
    divu    d0, d1
; Rouding for long average 
    clr     d0
    move.w  numLongs, d0
    move.l  d1, d2
    andi.l  #$0000FFFF, d1      ; D1 contains the dividend
    andi.l  #$FFFF0000, d2
    ror.l   #8, d2
    ror.l   #8, d2              ; D2 contains remainder
    move.l  d0, d3
    divu    #2, d3
    and.l   #$0000FFFF, d3      ; D3 has half of the divisor
    mulu    #100, d2
    add.l   d3, d2
    divu    d0, d2
    andi.l  #$0000FFFF, d2      ; D2 Contains the decimal term
    jsr     checkForProperRounding
; Displaying the average value
    lea averageLong, a1
    move.l  #DISPLAY_STRING_WITHOUT_CRLF_TRAP, d0
    trap    #15 
    jsr     displayNumber
   
stopExecution:    
    STOP    #$2000
    
paddingByteExists:
; Since padding byte exists, increment the number of padding bytes
    add.b  #1, numberOfPaddingBytes
    rts
    
; Takes dividend in d1 and decimal term in d2 and displays the number on display    
displayNumber:
; Display the part before the decimal point
    move.l  #DISPLAY_NUMBER_TRAP, d0
    trap    #15
; Display the decimal point     
    lea     dotString , a1
    move.l  #DISPLAY_STRING_WITHOUT_CRLF_TRAP, d0
    trap    #15
; Check if a 0 is required after the decimal point        
    cmpi.l  #9, d2
    bgt     noZeroRequired
    move.l  #0, d1
    move.l  #DISPLAY_NUMBER_TRAP, d0
    trap    #15
; Display the part after the decimal point    
noZeroRequired: 
    move.l  d2, d1
    move.l  #DISPLAY_NUMBER_TRAP, d0
    trap    #15
; Move to the next line    
    lea emptyString, a1
    move.l  #DISPLAY_STRING_TRAP, d0
    trap    #15 
    rts   

; Checks if rounding is producing the right resu
checkForProperRounding:
    cmpi.l  #100, d2
    blt    noEditInRounding
    move.l  #0, d2
    add.l   #1, d1
noEditInRounding:
    rts    
    
inputBinaryFile:
    INCBIN  "OutputBinaryFile.txt"    
numBytes:
    ds.w    1 
numShorts:
    ds.w    1
numLongs:
    ds.w    1
numberOfPaddingBytes:
    ds.b    1    
getUserInput:
    dc.b    'Enter the section number:',0  
averageByte:
    dc.b    'Average of byte is: ',0 
averageShort:
    dc.b    'Average of short is: ',0 
averageLong:
    dc.b    'Average of long is: ',0     
; Used to move to next line in output window
emptyString:
    dc.b    '',0   
dotString:
    dc.b    '.',0    
       
    END    START











*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
