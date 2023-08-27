(define pi 3.14159265358979323846264338327950288419716939937510)

(define length
  (lambda (ls)
    (cond
      ((null? ls) 0)
      (else (+ 1 (length (cdr ls)))))))

(define sqrt 
  (lambda (x) 
    (pow x 0.5)))

(define square 
  (lambda (x) 
    (* x x)))

(define abs
  (lambda (n)
    (cond
      ((negative? n) (* n (- 1)))
      (else n))))

(define quotient
  (lambda (m n)
    (floor (/ m n))))

(define remainder
  (lambda (m n)
    (cond
      ((and (negative? m) (negative? n))
       (- m (* n (floor (/ m n)))))
      ((or (negative? m) (negative? n))
       (- m (* n (ceiling (/ m n)))))
      (else (modulo m n)))))

(define modulo
  (lambda (m n)
    (cond
      ((and (negative? m) (negative? n))
       (- m (* n (floor (/ m n)))))
       (else (- m (* n (floor (/ m n))))))))

(define caar
  (lambda (ls)
    (car (car ls))))

(define caaar
  (lambda (ls)
    (car (car ls))))

(define cadr
  (lambda (ls)
    (car (cdr ls))))

(define cdar
  (lambda (ls)
    (cdr (car ls))))

(define caadr
  (lambda (ls)
    (car (car (cdr ls)))))

(define cddr
  (lambda (ls)
    (cdr (cdr ls))))

(define caddr
  (lambda (ls)
    (car (cdr (cdr ls)))))

(define cadddr
  (lambda (ls)
    (car (cdr (cdr (cdr ls))))))

(define caddddr
  (lambda (ls)
    (car (cdr (cdr (cdr (cdr ls)))))))

(define cadddddr
  (lambda (ls)
    (car (cdr (cdr (cdr (cdr (cdr ls))))))))

(define cadar
  (lambda (ls)
    (car (cdr (car ls)))))

(define cadadr
  (lambda (ls)
    (car (cdr (car (cdr ls))))))

(define caddar
  (lambda (ls)
    (car (cdr (cdr (car ls))))))

(define map
  (lambda (callback lst)
    (if (null? lst)
      '()
      (cons (callback (car lst))
        (map callback (cdr lst))))))

(define append
  (lambda (ls1 ls2)
    (cond
      ((null? ls1) ls2)
      (else (cons (car ls1) (append (cdr ls1) ls2))))))

(define reverse
  (lambda (ls)
    (foldr (lambda (a r) (append r (list a))) '() ls)))

(define filter
  (lambda (p ls)
    (cond
      ((null? ls) '())
      ((p (car ls)) (cons (car ls) (filter p (cdr ls))))
      (else (filter p (cdr ls))))))

(define null?
  (lambda (ls)
    (eqv? ls '())))

(define positive?
  (lambda (n)
    (> n 0)))

(define negative?
  (lambda (n)
    (< n 0)))

(define sub1
  (lambda (n)
    (- n 1)))

(define add1
  (lambda (n)
    (+ n 1)))

(define zero?
  (lambda (n)
    (= n 0)))

(define not
  (lambda (b)
    (if b #f #t)))

(define-macro (delay exp)
  (lambda () exp))

(define-macro (force exp)
  (exp))

(define vector
  (letrec ((vector-helper
            (lambda (v l idx)
              (cond
                ((null? l) v)
                (else
                 (begin
                   (vector-set! v idx (car l))
                   (vector-helper v (cdr l) (+ idx 1))))))))
    (lambda args
                (let ((vec (make-vector (length args))))
                  (vector-helper vec args 0)))))

(define list
  (letrec ((list-helper
            (lambda (ls)
              (cond
                ((null? ls) '())
                (else (cons (car ls)
                            (list-helper (cdr ls))))))))
    (lambda args
                (list-helper args))))

(define list-get
  (lambda (i ls)
    (cond
      [(zero? i) (car ls)]
      [else (list-get (sub1 i) (cdr ls))])))

(define pair?
  (lambda (p)
    (cons? p)))

(define xor
  (lambda (p1 p2)
    (not (eqv? p1 p2))))

(define even?
  (lambda (n) (= (modulo n 2) 0)))

(define odd?
  (lambda (n) (= (modulo n 2) 1)))
  
(define memv
  (lambda (obj ls)
    (cond
      ((null? ls) #f)
      ((eqv? (car ls) obj) #t)
      (else (memv obj (cdr ls))))))

(define assp
  (lambda (p l)
    (cond
      ((null? l) #f)
      ((p (caar l)) (car l))
      (else (assp p (cdr l))))))

(define assv
  (lambda (e l)
    (cond
      ((null? l) #f)
      ((eqv? (caar l) e) (car l))
      (else (assv e (cdr l))))))

(define foldr
  (lambda (func end lst)
    (if (null? lst)
      end
      (func (car lst) (foldr func end (cdr lst))))))

(define foldl
 (lambda (proc init lst)
  (cond
    [(null? lst) init]
    [else
     (foldl proc
               (proc (car lst) init)
               (cdr lst))])))

(define andmap
  (lambda (p ls)
    (cond
      [(null? ls) #t]
      [(p (car ls)) (andmap p (cdr ls))]
      [else #f])))

(define file-read-lines!
  (lambda (f)
    (cond
      [(not (file-has-next? f)) '()]
      [else (cons (file-read-line! f) (file-read-lines! f))])))

(define true?
  (lambda (b)
    (eqv? #t b)))

(define false?
  (lambda (b)
    (eqv? #f b)))