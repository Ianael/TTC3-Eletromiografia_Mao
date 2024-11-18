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
    [JsonRequired]
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
        //[JsonRequired]
        //[JsonProperty("FirstEmg")]
        //public string FirstEmg { get; set; }

        /// <summary>
        /// Valor da leitura, em mV, da rota FlexEmg que contempla o músculo flexor
        /// </summary>
        [JsonRequired]
        [JsonProperty("FlexEmg")]
        public string FlexEmg { get; set; }

        /// <summary>
        /// Valor da leitura, em mV, da rota ExtEmg que contempla o músculo extensor
        /// </summary>
        [JsonProperty("ExtEmg")]
        public string ExtEmg { get; set; }

        /// <summary>
        /// Valor da leitura, em mV, da rota PolEmg que contempla o músculo do polegar
        /// </summary>
        [JsonProperty("PolEmg")]
        public string PolEmg { get; set; }

        /// <summary>
        /// Horário da leitura
        /// </summary>
        [JsonProperty("timestamp")]
        public string Timestamp { get; set; }
    }
}