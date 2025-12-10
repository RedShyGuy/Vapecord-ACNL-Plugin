.arm
.fpu vfp
.align(4)

.macro FUNCTION name
    .section .text.\name
    .global \name
    .type \name, %function
    .align 2
\name:
.endm

FUNCTION    BGRHook
    LDR         R1, [R6, #0xC8]     @do standard instruction
@Custom Hair Color
    CMP         R4, #0              @if R4 is 0 (aka hair color, i think)
    LDREQ       R8, =0x39395A       @Hair Color bgr value
    CMPEQ       R1, R8              @check so its really only hair
    LDREQ       R1, =BGR_HairVal
    LDREQ       R1, [R1]
@Custom Eye Color
    CMP         R4, #1              @if R4 is 1 (aka eye color, i think)
    LDREQ       R8, =0x3858B0       @eye Color bgr value
    CMPEQ       R1, R8              @check so its really only eye
    LDREQ       R1, =BGR_EyeVal
    LDREQ       R1, [R1]

    MOV         R8, #0              @restore R8
    BX          LR                  @return

FUNCTION    MoveFurn
    BL          __IsIndoors  @data which holds check for if indoors 
    BX          LR

FUNCTION    PATCH_MoveFurnButton
    PUSH        {R1-R4, LR}
    PUSH        {R0}                @save R0 seperately
    BL          __IsPlayerHouse     @calls function to check if player is in house

    CMP         R0, #0              @if it is not player house
    POP         {R0}                @restore R0
    MOVEQ       R5, #0              @writes 0 to R5 if player isnt in house
    LSRNE       R5, R0, #0x1F       @do standard instruction
    
    POP         {R1-R4, PC}

FUNCTION    PATCH_KappnBypass1      @0x005DC048 (0xEB061CE9)
    PUSH        {R0}
    LDR         R0, =0x3E8
    CMP         R0, R1              @if Kappn (checked by money amount)
    MOVEQ       R5, #0
    STR         R5, [R6, #0]
    POP         {R0}
    BX          LR

FUNCTION    PATCH_KappnBypass2      @0x005DAF98 (0xEB06211D)
    PUSH        {R1}
    LDR         R1, =0x3E8
    CMP         R1, R0              @if Kappn (checked by money amount)
    MOVEQ       R0, #0
    LDRNE       R0, [SP, #0]
    POP         {R1}
    BX          LR

FUNCTION    PATCH_EverythingSeed
    LDR         R1, =EverythingSeederItemID
	BX          LR

FUNCTION    PATCH_PickupSeed        @0x59A22C | 0x598D7C
    LDR         R3, =PickupSeederItemID
    BX          LR

FUNCTION    PATCH_PartyPop          @0x671694 | 0x6716C0
    LDR         R1, =PartyPopperTool
    BX          LR

FUNCTION    PATCH_SnakeSpeed        @0x68E614
    PUSH        {R0-R4, LR}

    MOV         R0, R1
    BL          __IsPickSnakeID
    CMP         R0, #1              @if true
    LDREQ       R0, =0x50000000     @set speed
    VMOVEQ      S1, R0

    VMOV.F32    S17, S1             @execute standard instruction
    POP         {R0-R4, PC}

FUNCTION    PATCH_ToolAnim          @0x64DBA0
    PUSH        {LR}

    PUSH        {R0-R5}
    MOV         R0, R1
    BL          __IsAnimID          @if tool anim ID

    CMP         R0, #1              @if true
    POP         {R0-R5}
    LDREQ       R1, =toolTypeAnimID
    LDREQ       R1, [R1]

    MOV         R5, R1              @standard instruction
    POP         {PC}

FUNCTION    CHECK_IfFruit
    PUSH        {R1-R7, LR}
    MOV         R0, R7
    BL          __IsFruit
    CMP         R0, #1
    BLEQ        __AddFruit
    
    MOV         R0, R7
    POP         {R1-R7, PC}

FUNCTION    SetPlayerIconCoordinates
    PUSH        {LR}

    VLDR.F32    S0, =0xFFFFFFFF
    VLDR.F32    S1, =0xFFFFFFFF
    VLDR.F32    S2, =0xFFFFFFFF

    MOV         R0, R5
    POP         {PC}

FUNCTION    SetWalkParticleID
    PUSH        {LR}

    LDR         R4, =WalkParticleID
    LDR         R4, [R4]

    POP         {PC}

FUNCTION    SetEyeExpression
    PUSH        {R0, LR}

    BL          isIndividual
    CMP         R0, #1
    LDREQ       R1, =c_eyeID
    LDREQ       R1, [R1]

    POP         {R0}
    MOV         R5, R0
    POP         {PC}

FUNCTION    SetMouthExpression
    PUSH        {R0, LR}

    BL          isIndividual
    CMP         R0, #1
    LDREQ       R1, =c_mouthID
    LDREQ       R1, [R1]

    POP         {R0}
    MOV         R4, R0
    POP         {PC}    

FUNCTION    SetProperParticle
    PUSH        {R0-R3, LR}
    BL          __IsPuzzleLeagueRoom
    CMP         R0, #1
    LDREQ       R12, [SP, #0x84] @+0x14 due to stack changes (preserve LR and R0-R3)
    MOVNE       R12, #0
    POP         {R0-R3, PC}