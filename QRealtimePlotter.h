/* Copyright Sebastian Haas <sebastian@sebastianhaas.info>. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef QREALTIMEPLOTTER_H_
#define QREALTIMEPLOTTER_H_

#include <QtCore>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

#include <sys/time.h>

#define MAX_CURVES 2
#define MAX_SAMPLES 100000

class QRealtimePlotter : public QwtPlot
{
    Q_OBJECT

public slots:
    void newSampleReceived(const struct timeval & tv, double sample, const QString & source_name);

public:
    QRealtimePlotter(QWidget *parent = NULL);

    typedef enum E_SCALE {
        E_SCALE_LEFT = 0,
        E_SCALE_RIGHT,
        E_MAX_SCALE
    } scale_t;

    /**
     * Change scale settings
     * @param scale which scale to change
     * @param lower_bound lower value bound
     * @param upper_bound upper value bound
     * @param unit string to describe the physical unit
     */
    void changeScale(scale_t scale, const double & lower_bound, const double & upper_bound, const QString & unit);

    /**
     * Change the interval of displayed items
     * @param interval_ms interval in milliseconds
     */
    void setTimeScale(const double & interval_ms);

private:
    struct {
        QwtPlotCurve *curve;
        double sample[MAX_SAMPLES];
        double timedata[MAX_SAMPLES];
        quint32 sample_count;
    } m_Curves[MAX_CURVES];
};

#endif /* QREALTIMEPLOTTER_H_ */
