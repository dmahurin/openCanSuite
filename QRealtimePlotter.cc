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
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_scale_widget.h>

#include "QRealtimePlotter.h"

/// Small helper to provide a date/time string for the bottom scale
class TimeScaleDraw : public QwtScaleDraw
{
public:
    TimeScaleDraw() {}

    virtual QwtText label(double v) const {
        return QDateTime::fromMSecsSinceEpoch(static_cast<quint64>(v)).time().toString("hh:mm:ss.zzz");
    }
};

QRealtimePlotter::QRealtimePlotter(QWidget *parent) : QwtPlot(parent)
{
    setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());
    setAxisLabelRotation(QwtPlot::xBottom, -50.0);
    setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    QwtScaleWidget *scaleWidget = axisWidget(QwtPlot::xBottom);
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, fmh / 2);

    setAxisTitle(QwtPlot::xBottom, "Time");

    m_Curves[0].curve = new QwtPlotCurve();
    m_Curves[0].curve->attach(this);
    m_Curves[0].sample_count = 0;

    setTimeScale(1000.0);
}

void QRealtimePlotter::changeScale(scale_t scale,
                                   const double & lower_bound,
                                   const double & upper_bound,
                                   const QString & unit)
{
    QwtPlot::Axis axis = scale == E_SCALE_LEFT ? QwtPlot::yLeft : QwtPlot::yRight;
    
    setAxisScale(axis, lower_bound, upper_bound);
    setAxisTitle(axis, unit);
}

void QRealtimePlotter::setTimeScale(const double & interval_ms)
{
    double now = static_cast<double>(QDateTime::currentDateTime().toMSecsSinceEpoch());

    setAxisScale(QwtPlot::xBottom, now, now + interval_ms);
}

void QRealtimePlotter::newSampleReceived(const struct timeval & tv, double sample, const QString & source_name)
{
    m_Curves[0].sample[m_Curves[0].sample_count] = sample;
    m_Curves[0].timedata[m_Curves[0].sample_count++] = (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);

    m_Curves[0].curve->setRawSamples(&m_Curves[0].timedata[0], &m_Curves[0].sample[0], m_Curves[0].sample_count);

    replot();
}
