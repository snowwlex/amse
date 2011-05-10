package ru.amse.turkin;


public interface Task<X,Y> {
    X run(Y value);
}
