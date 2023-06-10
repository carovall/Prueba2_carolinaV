
	1) El problema planteado corresponde a un problema de valor inicial de condiciones mixtas.

	2) Sí, es posible resolver el problema con los métodos vistos en clase, ya que las dos ecuaciones de segundo orden pueden reescribirse
	de forma tal que se obtienen 4 ecuaciones de primer orden, para las cuales se conocen los 4 valores iniciales pertinentes (posiciones y
	velocidades angulares de cada péndulo). De los métodos vistos en clase para resolución de ODEs escogería RK45, ya que al no tener un paso
	h fijo se obtendrían estimaciones más certeras, tomando en cuenta el carácter caótico que se espera del movimiento del péndulo doble para
	oscilaciones no pequeñas.

	3) No, ya que la función resultante aún estaría compuesta por funciones trigonométricas de theta y por lo tanto no sería lineal. Para el caso
	de los valores iniciales dados no sería válida la aproximación de sin(x) y cos(x) que se realiza para pequeñas oscilaciones.

	4) En el caso de que se tuviera una condición de frontera que se quiera imponer, el problema de valor de frontera puede ser resuelto ocupando
	shooting no lineal y el método de Newton-Raphson, el cual es útil para sistemas de ecuaciones no lineales, como en este caso.
