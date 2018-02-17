(load "env1.scm")

(define last-of-expr
  (lambda(expr)
    (cond
      ((null? expr) 
        '())
      ((null? (cdr expr))
        (car expr))
      (else
        (last-of-expr (cdr expr))
      )
    )
  )
)

;Checks if there any subexpressions in an expression
(define no-subexpr
  (lambda (expr)
    (cond
      ((null? expr) 
        #t)
      ((list? (car expr))
        #f)
      (else
        (no-subexpr (cdr expr))
      )
    )
  )
)

;Evaluates an infix expression e.g (1 + 1)
(define eval-expr
    (lambda (expr)
      (cond
        ((eq? (car expr) 'inc)
              (+ (cadr expr) 1))
        ((eq? (car expr) 'dec)
              (- (cadr expr) 1))
        ((eq? (cadr expr) '+) 
          (+ (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '-) 
          (- (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '*) 
          (* (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '/) 
          (/ (car expr) (last-of-expr expr)))
        ((eq? (cadr expr) '**) 
          (expt (car expr) (last-of-expr expr)))
      )
    )
)

(define is-operator
  (lambda (sym)
    (cond
      ((or (eq? sym '+) (eq? sym '-) (eq? sym '*) (eq? sym '/) (eq? sym '**) (eq? sym 'inc) (eq? sym 'dec))
        #t)
      (else
        #f
      )
    )
  )
)

;Evaluates nested infix expressions e.g. (1 + (1 / 2))
(define eval-nested
  (lambda (expr)
    (cond
        ((no-subexpr expr) 
          (eval-expr expr))
        ((= (length expr) 2)
          (eval-expr (list (car expr) (eval-nested (cadr expr)))))
        ((and (list? (car expr)) (list? (last-of-expr expr)))
          (eval-expr (list (eval-nested (car expr)) (cadr expr) (eval-nested (last-of-expr expr)))))
        ((list? (car expr))
           (eval-expr (list (eval-nested (car expr)) (cadr expr) (last-of-expr expr))))
        (else
          (eval-expr (list (car expr) (cadr expr) (eval-nested (last-of-expr expr))))
        )
    )
  )
)

(define (deep-map f l)
  (define (deep x)
    (cond ((null? x) x)
          ((pair? x) (map deep x))
          (else (f x))))
  (map deep l))

; (define replace-v-val
;   (lambda (expr)

;   )
; )

; (define myeval
;   (lambda (expr env)

;   )
; )