# CYML

```
TUPLE creature BEGIN
    STRING name "Frog" 
    TUPLE attributes BEGIN
        TUPLE life_attribute BEGIN
            U32 health 10
            U32 max_health 10
        END
        TUPLE physical_attribute BEGIN
            F32 width 1.2
            F32 height 2.4
            F32 length 3.6
            F32 mass 5.1
        END
    END
END
```