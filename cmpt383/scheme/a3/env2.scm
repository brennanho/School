(define make-empty-env
    (lambda ()
      '()
    )
)

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
        (error "apply-env: empty environment or element not found"))
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
                (extend-env 'b 22
                    (make-empty-env)))))
)