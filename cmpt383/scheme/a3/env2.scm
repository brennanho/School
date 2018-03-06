;env2 implementation is a list of alternating name and associated values e.g. (a 1 b 2 c 3)

(define make-empty-env
    (lambda ()
      '()
    )
)

;HELPER - Checks if a variable is already in the environment
(define v-in-env
  (lambda (env v)
    (cond
      ((null? env)
        #f)
      ((equal? (car env) v)
        #t)
      (else
        (v-in-env (cdr (cdr env)) v)     
      )
    )
  )  
)

;HELPER - Updates the value of a name value pair with val
(define update-v-in-env
  (lambda (env v val)
    (cond
      ((null? env)
        env)
      ((equal? (car env) v)
        (cons (car env) (cons val (update-v-in-env (cdr (cdr env)) v val))))
      (else
        (cons (car env) (cons (car (cdr env)) (update-v-in-env (cdr (cdr env)) v val)))
      )
    )
  )  
)

(define extend-env
  (lambda (v val env)
    (cond
      ((v-in-env env v)
        (update-v-in-env env v val))
      (else
        (cons v (cons val env))
      )
    )
  )
)

(define apply-env
  (lambda (env v)
    (cond
      ((null? env)
        (error "apply-env: empty environment or element not found:" v))
      ((equal? (car env) v)
        (car (cdr env)))
      (else
        (apply-env (cdr (cdr env)) v)
      )
    )
  )  
)

(define test-env
    (extend-env 'a 1
        (extend-env 'b 2
            (extend-env 'c 3
                (extend-env 'b 4
                    (make-empty-env)))))
)

(define env1
    (extend-env 'x -1
        (extend-env 'y 4
            (extend-env 'x 1
                (make-empty-env))))
)

(define env2
    (extend-env 'm -1
        (extend-env 'a 4
            (make-empty-env)))
)

(define env3
    (extend-env 'q -1
        (extend-env 'r 4
            (make-empty-env)))
)