RES 1 "Tic - Tac - Dos\\"
RES 2 "___|___|___\"
RES 3 "   |   |   \"
RES 4 "Player "
RES 5 "X"
RES 6 "O"
RES 7 " turn"
RES 8 " won!"
RES 9 "\"
LDI 1 49 //1
LDI 2 50 //2
LDI 3 51 //3
LDI 4 52 
LDI 5 53
LDI 6 54
LDI 7 55
LDI 8 56
LDI 9 57
LDI 11 0 //Player //Player O è 1 //Player X è 0
LDI 12 0 //Azione completata
LDI 13 0
LDI 14 1
LDI 15 9
CLS //GRAFICA
WRT_T 1
WRT_T 3
WRT_T 3
WRT_T 2
WRT_T 3
WRT_T 3
WRT_T 2
WRT_T 3
WRT_T 3
WRT_T 3
WRT_T 9
WRT_T 4
JMP_OD 3 11 0 //Se player = 1 passa alla giusta scrittura
WRT_T 5 1
JMP_OE 2 //Salta la scrittura della O
WRT_T 6 1
WRT_T 7
KEY 10 //Chiede l'input
JMP_OE -1 10 0 //Lo chiede finche non lo riceve
JMP_OD 4 10 1
CRS_I 6 24 //Va al centro della casella 1, 1
LDI 1 0 //Blocca quella casella
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 2
CRS_I 30 24 //Va al centro della casella 2, 1
LDI 2 0
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 3
CRS_I 54 24 //Va al centro della casella 3, 1
LDI 3 0
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 4
CRS_I 6 48 //Va al centro della casella 1, 2
LDI 4 0
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 5
CRS_I 30 48 //Va al centro della casella 2, 2
LDI 5 0
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 6
CRS_I 54 48 //Va al centro della casella 3, 2
LDI 6 0
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 7
CRS_I 6 72 //Va al centro della casella 1, 3
LDI 7 0
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 8
CRS_I 30 72 //Va al centro della casella 2, 3
LDI 8 0
LDI 12 1 //Ha fatto un azione
JMP_OD 4 10 9
CRS_I 54 72 //Va al centro della casella 3, 3
LDI 9 0
LDI 12 1 //Ha fatto un azione
JMP_LE 42 12 0
JMP_OD 4 11 0 //Se player = 1 passa alla giusta scrittura
WRT_T 5 1
LDI 11 1
JMP_OE 3 //Salta la scrittura della O
WRT_T 6 1
LDI 11 0
CRS_I 0 96 //Ritorna ALLA scritta del giocatore
LDI 12 0 //resetta l'azione
ADD 13 13 14
JMP_OE 2 13 15 //Ritorna alla riga dove scrive il giocatore
JMP_LE 36
END