package ru.amse.turkin;


public interface TaskRunner {
    public <X, Y> X run(Task<X, Y> task, Y value);
}
