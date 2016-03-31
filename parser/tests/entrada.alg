Algoritmo "Baskhara"
Var
   v:vetor[5..15] de inteiro
   i:inteiro
   a, b, c, x1, x2, delta : real
Inicio
   para i de 1 ate 20 faca
      escreva(v[i+1])
   fimpara
   enquanto i>0 faca
      i<-i-1
   fimenquanto
   v[i+1]<-v[i+2]
   repita
      i<-i+1
      v[10]<-18
   ate a>b
   se a>b e b>c
      entao escreva(a)
      senao escreva(b)
   fimse
   escreva("Digite os coeficientes a, b e c:")
   leia(a)
   leia(b)
   leia(c)
   delta <- (b^2-4*a*c)^0.5
   x1 <- (-b+delta)/(2*a)
   x2 <- (-b-delta)/(2*a)
   escreva ("As raízes são ")
   escreva (x1)
   escreva (" e ")
   escreval (x2)
fimalgoritmo

