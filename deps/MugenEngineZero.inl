//------------------------------------------------------------------------------ Debug Methods
template <class E, class M> E log(E error, M msg)
{
    #ifndef HIDE_LOG
    std::cout << msg;
    #endif // HIDE_LOG
    return error;
}
