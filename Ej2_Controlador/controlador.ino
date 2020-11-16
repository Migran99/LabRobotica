float tAct, *pRk;
float controlador(float *pRkm, float Yk, float Tm)
{
  float Rk = 0;
  float Uk, Uks; //Señal de control y señal de control saturada
  float uk; //Señal de control incremental
  float Ueq = 0;
  static float Int_k = 0.0, Int_ks = 0.0;
  static float ek = 0.0, e_k_1 = 0.0;
  float Umax = 10.0, Umin = 0.0; /* Valores límite de la señal de control */
  float kp = 1.522, Ti = 0.5, Td = 0.0;
  float Tt = 0.3;

  /*Control*/
  if (tAct < 5.)
    Uk = Ueq;
  else //Activamos el control cuando la salida ha tomado un valor similar a la referencia
  {
    ek = Rk - Yk; //Cálculo del error actual

    //Calculamos la señal de control incremental (PID)
    uk = kp * (ek + (1. / Ti) * Int_k + Td * (ek - e_k_1) / Tm);

    e_k_1 = ek; //Actualizamos el error del instante anterior
    Int_k += ek * Tm; //Aproximación de la integral del error (Euler II) -- No sería Euler I? Me afecta en el siguiente ciclo, donde habré tomado ek_1*Tm

    Uk = Ueq + uk + Int_ks / Tt; //Señal de control absoluta

    /* Saturación de la señal de control */
    if (Uk > Umax) Uks = Umax;
    else if (Uk < Umin) Uks = Umin;
    else Uks = Uk;

    Int_ks += (Uks - Uk) * Tm;



    /* Antes de salir: escribir referencia */
    *pRk = Rk;
    return Uks;
  }
}
