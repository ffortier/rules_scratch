# Scratch-Lisp

Lisp syntax for scratch. The idea is to emulate the blocks as much as possible.

```lisp
;; Block definition
(define sayHello "Say hello to (name) <casual>"
    (if casual
        (then
            (sayFor (join "Yo " (join name "!" ) 2 ))
        )
        (else
            (sayFor (join "Hello " (join name "!" ) 2 ))
        )
    )

    (if casual
        (sayFor "OMG!")
    )
)

;; (container ...args ...statements)
;; (statement ...args)
;; (container ...args (body1 ...statements) (body2 ...statements) ...statements)
```