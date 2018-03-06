(load "env2.scm")

;HELPER - Returns the last value of an expression e.g. returns 3 if the expression is (x + 3) or 2 if the expression is inc 2
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

;HELPER - Checks whether an expression contains any subexpressions, returns a boolean value
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

;HELPER - Evaluates an expression that does not contain any subexpressions e.g. (2 * x)
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

;HELPER - Replaces all variable names in an expression with their associated values from their environment e.g. if x = 3, then (3 + x) will be converted to (3 + 3)
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

;HELPER - Same as the above replace-v-val function except is used for a more general case with nested expressions
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

;HELPER - Evaluates nested infix expressions e.g. (1 + (1 / 2))
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

;Required for part 2
(define myeval
  (lambda (expr env)
    (eval-nested (replace-v-val-nested env expr))
  )
)