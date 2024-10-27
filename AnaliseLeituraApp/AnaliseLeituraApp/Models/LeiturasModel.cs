using Newtonsoft.Json;

public class LeiturasModel
{
    /// <summary>
    /// Lista de propriedades dos gráficos
    /// </summary>
    [JsonProperty("charts")]
    public Chart Charts { get; set; }

    /// <summary>
    /// Lista das leituras
    /// </summary>
    [JsonProperty("readings")]
    public List<Data> Readings { get; set; }


    public class Chart
    {
        /// <summary>
        /// Número de leituras exibidas no gráfico
        /// </summary>
        [JsonProperty("range")]
        public string Range { get; set; }
    }

    public class Data
    {
        /// <summary>
        /// Valor da leitura, em mV, da rota FirstEmg que contempla o músculo flexor
        /// </summary>
        [JsonProperty("FirstEmg")]
        public string FirstEmg { get; set; }

        /// <summary>
        /// Horário da leitura
        /// </summary>
        [JsonProperty("timestamp")]
        public string Timestamp { get; set; }
    }
}