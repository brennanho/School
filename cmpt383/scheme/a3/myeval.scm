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

(define no-subexpr?
  (lambda (expr)
    (cond
      ( (and (not (list? (car expr))) (not (list? (cadr expr))) (not (list? (last-of-expr expr))))
        #t)
      (else
        #f)
    )
  )
)

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

(define replace-v-val
  (lambda (env v)
    (cond
      ((null? v)
        v)
      ((or (eq? (car v) '+) (eq? (car v) '-) (eq? (car v) '*) (eq? (car v) '/) (eq? (car v) '**) (eq? (car v) 'inc) (eq? (car v) 'dec) (number? (car v)))
          (cons (car v) (replace-v-val env (cdr v))))
      (else
        (cons (apply-env env (car v)) (replace-v-val env (cdr v)))
      )
    )
  )
)

(define is-operator
  (lambda (x)
    (cond
      ((or (eq? x '+) (eq? x '-) (eq? x '/) (eq? x '*) (eq? x '**) (eq? x 'inc) (eq? x 'dec))
        #t)
      (else
        #f
      )
    )
  )
)
; )

(define replace-v-val-nested
  (lambda (env v)
    (cond
      ((no-subexpr? v)
        (replace-v-val env v))
      ((list? (car v))
        (list (replace-v-val-nested env (car v)) (cadr v) (last-of-expr v)))
      (else
        (list (car v) (cadr v) (replace-v-val-nested env (last-of-expr v)))
      )
    )  
  )  
)

;Evaluates nested infix expressions e.g. (1 + (1 / 2))
(define eval-nested
  (lambda (expr)
    (cond
        ((no-subexpr? expr) 
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

(define myeval
  (lambda (expr env)
    (eval-nested (replace-v-val-nested env expr))
  )
)